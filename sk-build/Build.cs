using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;
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
    //const string LinuxUserBins = "/usr/bin";
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
        string[] allowedExtensions = { ".c", ".cpp", ".asm", ".s", ".rs" };
        var fileList = Directory.GetFiles(dir, "*.*", SearchOption.AllDirectories)
                             .Where(file => allowedExtensions.Contains(Path.GetExtension(file)))
                             .ToArray();
        foreach (string f in fileList)
        {
            FilePaths.Add(f);
        }
        return FilePaths;
    }
    static List<string> GetObjectFilesRecursivelyInDirectory(string dir)
    {
        List<string> FilePaths = new();
        var fileList = Directory.GetFiles(dir, "*.o", SearchOption.AllDirectories);
        foreach (string f in fileList)
        {
            if (dir.Contains(' '))
                FilePaths.Add($"\"{f}\"");
            else
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
            string s2 = s;
            if (s.Contains(' '))
                s2 = $"\"{s2}\"";

            string obj = s2.Replace("src", "obj-" + arch);
            bool canBeCompiled = true;

            if (File.Exists(s2.Replace(srcDir, tempSourceTree)))
            {
                string savedText = File.ReadAllText(s.Replace(srcDir, tempSourceTree));
                string currentText = File.ReadAllText(s);

                if (savedText == currentText)
                    canBeCompiled = false;
            }

            string topCDef = File.ReadAllLines(s)[0];

            if (s.EndsWith(".c") || s.EndsWith(".cpp"))
            {
                obj = obj.Replace(".c", ".o");

                if (!canBeCompiled)
                    if (!File.Exists(obj))
                        canBeCompiled = true;

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
                    p.StartInfo = new($"{arch.ToString().ToLower()}-elf-g{(s.EndsWith(".cpp") ? "++" : "cc")}", string.Join(' ', ccargs) + $" -o {obj} -c {s2}");
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
            else if (s.EndsWith(".s"))
            {
                Console.WriteLine("[INFO] GNU ASM files '.s' compilation isn't supported right now.");
            }
            else if (s.EndsWith(".rs"))
            {
                Console.WriteLine("[INFO] Rust files '.rs' compilation isn't supported right now.");
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
                    Console.WriteLine($"[ASM] {s} => {obj}");
                    var p = Process.Start("nasm", $"{s2} -felf64 -o {obj}");
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

    public static string Link(Architecture arch)
    {
        string linkScriptPath = Path.Combine(tempDir, "lnk-" + arch + ".ld");
        string outputKernelPath = "";
        
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
            "-z max-page-size=0x1000",
            string.Join(' ', GetObjectFilesRecursivelyInDirectory(objDirStart + arch))
        };

        if (outputDir.Contains(' '))
            ldArgs.Add($" -o \"{Path.Combine(outputDir, outputKernelPath)}\"");
        else
            ldArgs.Add($" -o {Path.Combine(outputDir, outputKernelPath)}");

        if (linkScriptPath.Contains(' '))
            ldArgs.Add($"-T \"{linkScriptPath}\"");
        else
            ldArgs.Add($"-T {linkScriptPath}");

        Console.WriteLine("[LD] " + outputKernelPath);

        //Console.WriteLine(string.Join(' ', ldArgs));

        var p = Process.Start($"{arch.ToString().ToLower()}-elf-ld", string.Join(' ', ldArgs));
        while (!p.HasExited)
            ;;
        
        if (p.ExitCode != 0)
            Console.WriteLine("[FAIL] Compilation failed. Please check the error(s) shown above.");

        return outputKernelPath;
    }

    public static void Build(Architecture arch)
    {
        List<string> CCArgs = new() {
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
            "-g"
        };

        if (arch == Architecture.x86_64)
        {
            CCArgs.Add("-m64");
            CCArgs.Add("-mabi=sysv");
            CCArgs.Add("-march=x86-64");
            CCArgs.Add("-mno-80387");
            CCArgs.Add("-mno-mmx");
            CCArgs.Add("-mno-sse");
            CCArgs.Add("-mno-sse2");
            CCArgs.Add("-mno-red-zone");
            CCArgs.Add("-mcmodel=kernel");
        }
        
        PrepDirs(arch);

        Console.WriteLine("[READY] Starting SipaaKernel compilation for " + arch);

        AddConfigDefinesToArguments(CCArgs);
        if (BuildSourceCode(arch, CCArgs) == null)
            Environment.Exit(1);
        Link(arch);

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