using System.Diagnostics;
using System.Reflection;
using Terminal.Gui;

namespace SipaaKernel.Builder;

public enum Architecture
{
    AArch64,
    x86_64,
    RiscV64
}

public class Builder
{
    const string LinuxUserBins = "/usr/bin";
    static string objDirStart = "obj-";
    static string outputDir = Path.Combine(Environment.CurrentDirectory, "output");
    static string srcDir = Path.Combine(Environment.CurrentDirectory, "src");
    static string tempDir = Path.Combine(Environment.CurrentDirectory, ".skb");
    static string tempSourceTree = Path.Combine(tempDir, "srctree");

    static void AddConfigDefinesToArguments(List<string> args)
    {
        string defineStart = "-D";
        Type configType = typeof(SKConfig);
        PropertyInfo[] properties = configType.GetProperties();
        foreach (PropertyInfo property in properties)
        {
            if (!property.GetGetMethod().IsStatic)
            {
                object value = property.GetGetMethod().Invoke(SKConfig.Current, null);
                if (value.GetType() == typeof(Boolean))
                    if (((bool)value) == true)
                        args.Add(defineStart + "SKC_" + (property.Name.ToUpper()));
            }
        }
    }

    static List<string> GetSourceFilesRecursivelyInDirectory(string dir)
    {
        List<string> FilePaths = new();
        string[] allowedExtensions = { ".c", ".cpp", ".asm", ".s" };
        var fileList = Directory.GetFiles(dir, "*.*", SearchOption.AllDirectories)
                             .Where(file => allowedExtensions.Contains(Path.GetExtension(file)))
                             .ToArray();
        foreach (string f in fileList)
        {
            FilePaths.Add(f);
        }
        return FilePaths;
    }

    static void PrepDirs(Architecture arch)
    {
        Console.WriteLine("[PREP] Creating object directory structure for " + arch);

        if (!Directory.Exists(objDirStart + arch.ToString()))
            Directory.CreateDirectory(objDirStart + arch.ToString());

        if (!Directory.Exists(outputDir))
            Directory.CreateDirectory(outputDir);

        if (!Directory.Exists(tempDir))
            Directory.CreateDirectory(tempDir);

        if (!Directory.Exists(tempSourceTree))
            Directory.CreateDirectory(tempSourceTree);

        foreach (string d in Directory.GetDirectories(srcDir, "*", SearchOption.AllDirectories))
        {
            string objCounterpart = d.Replace("src", "obj-" + arch);
            if (!Directory.Exists(objCounterpart))
                Directory.CreateDirectory(objCounterpart);
        }
    }

    static List<string> BuildSourceCode(Architecture arch, List<string> ccargs)
    {
        List<string> objs = new();

        foreach (string s in GetSourceFilesRecursivelyInDirectory(Path.Combine(Environment.CurrentDirectory, "src")))
        {
            string obj = s.Replace("src", "obj-" + arch);
            bool canBeCompiled = true;

            if (File.Exists(s.Replace(srcDir, tempSourceTree)))
            {
                string savedText = File.ReadAllText(s.Replace(srcDir, tempSourceTree));
                string currentText = File.ReadAllText(s);

                if (savedText == currentText)
                    canBeCompiled = false;
            }

            string topCDef = File.ReadAllLines(s)[0];

            if (s.EndsWith(".c"))
            {
                obj = obj.Replace(".c", ".o");

                if (topCDef != "// SKB_ARCH_INDEPENDANT")
                {
                    if (topCDef == "// SKB_X86_64_ONLY" && arch != Architecture.x86_64)
                        canBeCompiled = false;
                    if (topCDef == "// SKB_AARCH64_ONLY" && arch != Architecture.AArch64)
                        canBeCompiled = false;
                    if (topCDef == "// SKB_RISCV64_ONLY" && arch != Architecture.RiscV64)
                        canBeCompiled = false;
                }

                if (canBeCompiled)
                {
                    Console.WriteLine($"[CC] {s} => {obj}");
                    Process p = new();
                    p.StartInfo = new(Path.Combine(LinuxUserBins, $"{arch.ToString().ToLower()}-elf-gcc"), string.Join(' ', ccargs) + $" -o {obj} -c {s}");
                    p.Start();

                    while (!p.HasExited)
                        ;;

                    if (p.ExitCode != 0)
                    {    
                        Console.WriteLine("[FAIL] Compilation failed. Please check the error(s) shown above.");
                        return null;
                    }

                    objs.Add(obj);
                }
            }
            else if (s.EndsWith(".asm") && arch == Architecture.x86_64)
            {
                obj = obj.Replace(".asm", "-asm.o");
                
                if (topCDef != "; SKB_ARCH_INDEPENDANT")
                {
                    if (topCDef == "; SKB_X86_64_ONLY" && arch != Architecture.x86_64)
                        canBeCompiled = false;
                    if (topCDef == "; SKB_AARCH64_ONLY" && arch != Architecture.AArch64)
                        canBeCompiled = false;
                    if (topCDef == "; SKB_RISCV64_ONLY" && arch != Architecture.RiscV64)
                        canBeCompiled = false;
                }

                if (canBeCompiled)
                {
                    var p = Process.Start(Path.Combine(LinuxUserBins, "nasm"), $"{s} -felf64 -o {obj}");
                    while (!p.HasExited)
                        ;;

                    if (p.ExitCode != 0)
                    {    
                        Console.WriteLine("[FAIL] Compilation failed. Please check the error(s) shown above.");
                        return null;
                    }

                    objs.Add(obj);
                }
            }
        }

        return objs;
    }

    public static string Link(Architecture arch, List<string> objs)
    {
        string linkScriptPath = Path.Combine(tempDir, "lnk-" + arch + ".ld");
        string outputKernelPath = "";

        if (objs.Count == 0)
            return outputKernelPath;
        
        outputKernelPath = $"kernel-{arch.ToString().ToLower()}.elf";

        // Download link script
        if (!File.Exists(linkScriptPath))
        {
            HttpClient hc = new();
            FileStream s = File.Create(linkScriptPath);
            string linkScriptUrl = $"https://raw.githubusercontent.com/limine-bootloader/limine-c-template-portable/trunk/kernel/linker-{arch.ToString().ToLower()}.ld";
            Console.WriteLine($"[LD] Downloading '{linkScriptUrl}', necessary to link the kernel.");
            var downloadedLinkScript = hc.GetStreamAsync(linkScriptUrl);
            downloadedLinkScript.Result.CopyTo(s);
            s.Close();
        }

        List<string> ldArgs = new() {
            "-nostdlib",
            "-static",
            $"-T {linkScriptPath}",
            "-z max-page-size=0x1000",
            string.Join(' ', objs),
            $" -o {Path.Combine(outputDir, outputKernelPath)}"
        };

        Console.WriteLine("[LD] " + outputKernelPath);

        var p = Process.Start(Path.Combine(LinuxUserBins, $"{arch.ToString().ToLower()}-elf-ld"), string.Join(' ', ldArgs));
        while (!p.HasExited)
            ;;
        
        if (p.ExitCode != 0)
            Console.WriteLine("[FAIL] Compilation failed. Please check the error(s) shown above.");

        return outputKernelPath;
    }

    public static void Build(Architecture arch)
    {
        List<string> x8664CCArgs = new() {
            "-ffreestanding",
            "-w",
            "-Dlimine",
            "-std=gnu++11",
            "-ffreestanding",
            "-fno-stack-protector",
            "-fpermissive",
            "-fno-stack-check",
            "-fno-lto",
            "-fno-PIE",
            "-fno-PIC",
            "-Isrc/kernel/",
            "-Isrc/entry",
            "-Isrc/kernel/sk-hal",
            "-Isrc/libs",
            "-Isrc/libs/slibc",
            "-g",
            "-m64",
            "-mabi=sysv",
            "-march=x86-64",
            "-mno-80387",
            "-mno-mmx",
            "-mno-sse",
            "-mno-sse2",
            "-mno-red-zone",
            "-mcmodel=kernel"
        };
        

        PrepDirs(arch);

        Console.WriteLine("[READY] Starting SipaaKernel compilation for " + arch);

        AddConfigDefinesToArguments(x8664CCArgs);

        Link(arch, BuildSourceCode(arch, x8664CCArgs));

        Console.WriteLine("[PREP] Copying source tree to temp");

        foreach (string d in Directory.GetDirectories(srcDir, "*", SearchOption.AllDirectories))
        {
            string copy = d.Replace(srcDir, tempSourceTree);
            if (!Directory.Exists(copy))
                Directory.CreateDirectory(copy);
        }

        foreach (string s in GetSourceFilesRecursivelyInDirectory(Path.Combine(Environment.CurrentDirectory, "src")))
        {
            File.Copy(s, s.Replace(srcDir, tempSourceTree), true);
        }
    }
}