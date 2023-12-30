using System.Diagnostics;
using System.Reflection;

namespace SipaaKernel.Builder;

public class Builder
{
    static bool FileLineCheck(string topCDef, string commentPrefix, Architecture arch)
    {
        bool canBeCompiled = true;

        if (!topCDef.Contains($"SKB_ARCH_INDEPENDANT"))
        {
            if (topCDef.Contains("SKB_X86_64_ONLY") && arch != Architecture.x86_64)
                canBeCompiled = false;
            if (topCDef.Contains("SKB_AARCH64_ONLY") && arch != Architecture.AArch64)
                canBeCompiled = false;
            if (topCDef.Contains("SKB_RISCV64_ONLY") && arch != Architecture.RiscV64)
                canBeCompiled = false;
            if (topCDef.Contains("SKB_i686_ONLY") && arch != Architecture.i686)
                canBeCompiled = false;
            if (topCDef.Contains("SKB_NO_X86_64") && arch == Architecture.x86_64)
                canBeCompiled = false;
            if (topCDef.Contains("SKB_NO_AARCH64") && arch == Architecture.AArch64)
                canBeCompiled = false;
            if (topCDef.Contains("SKB_NO_RISCV64") && arch == Architecture.RiscV64)
                canBeCompiled = false;
            if (topCDef.Contains("SKB_NO_i686") && arch == Architecture.i686)
                canBeCompiled = false;
        }

        return canBeCompiled;
    }

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
    
    static void PrepDirs(Architecture arch)
    {
        Console.WriteLine("[PREP] Creating object directory structure for " + arch);

        if (!Directory.Exists(Constants.ObjectDirectoryStart + arch.ToString()))
            Directory.CreateDirectory(Constants.ObjectDirectoryStart + arch.ToString());

        if (!Directory.Exists(Constants.OutputDirectory))
            Directory.CreateDirectory(Constants.OutputDirectory);

        if (!Directory.Exists(Constants.TempDirectory))
            Directory.CreateDirectory(Constants.TempDirectory);

        if (!Directory.Exists(Constants.TempSourceTree + $"-{arch}"))
            Directory.CreateDirectory(Constants.TempSourceTree + $"-{arch}");

        foreach (string d in Directory.GetDirectories(Constants.SourceDirectory, "*", SearchOption.AllDirectories))
        {
            string objCounterpart = d.Replace("src", "obj-" + arch);
            if (!Directory.Exists(objCounterpart))
                Directory.CreateDirectory(objCounterpart);
        }
    }

    public static int Build(BuildOptions opt)
    {
        Toolchain t = Enum.Parse<Toolchain>(opt.Toolchain);
        Architecture a = Enum.Parse<Architecture>(opt.Architecture);

        List<string> source = IOUtils.GetFilesRecursivelyInDirectory(Constants.SourceDirectory, new string[] { ".c", ".cpp", ".rs", ".s", ".asm" });
        bool cc = true;
        string outp = "";
        List<string> CompiledSourceFiles = new();

        List<string> CCArgs = new();

        if (t == Toolchain.Llvm)
            CCArgs.Add($"-target {opt.Architecture.ToLower()}-none-elf");
        CCArgs.Add("-Isrc/kernel/");
        CCArgs.Add("-Isrc/entry");
        CCArgs.Add("-Isrc/kernel/sk-hal");
        CCArgs.Add("-ffreestanding");
        CCArgs.Add("-Isrc/libs");
        CCArgs.Add("-Isrc/libs/slibc");
        CCArgs.Add("-ffreestanding");
        CCArgs.Add("-fno-stack-protector");
        CCArgs.Add("-fpermissive");
        CCArgs.Add("-fno-pic");
        CCArgs.Add("-fno-stack-check");
        CCArgs.Add("-fno-lto");
        CCArgs.Add("-fno-PIE");
        CCArgs.Add("-g");
        CCArgs.Add("-w");
        CCArgs.Add("-Dlimine");

        if (a == Architecture.x86_64)
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
        else if (a == Architecture.i686)
        {
            CCArgs.Add("-O1");
            CCArgs.Add("-m32");
            CCArgs.Remove("-Dlimine");
        }

        ///// Prepare the build
        PrepDirs(a);
        AddConfigDefinesToArguments(CCArgs);

        ///// Build the source code
        foreach (string s in source)
        {
            cc = true;

            if (File.Exists(s.Replace(Constants.SourceDirectory, Constants.TempSourceTree + "-" + a)))
            {
                string savedText = File.ReadAllText(s.Replace(Constants.SourceDirectory, Constants.TempSourceTree + "-" + a));
                string currentText = File.ReadAllText(s);

                if (savedText == currentText)
                    cc = false;
            }

            string[] content = File.ReadAllLines(s);

            if (s.EndsWith(".c"))
            {
                outp = s.Replace(".c", ".o").Replace(Constants.SourceDirectory, Constants.ObjectDirectoryStart + a);

                if (cc)
                    cc = FileLineCheck(content[0], "// ", a);

                if (cc)
                {
                    Console.WriteLine($"[CC] {s} => {outp}");
                    
                    Process pr = new();
                    if (t == Toolchain.Gnu)
                        pr.StartInfo = new($"{a.ToString().ToLower()}-elf-g{(s.EndsWith(".cpp") ? "++" : "cc")}", string.Join(' ', CCArgs) + $" -o {outp} -c {s}");
                    else if (t == Toolchain.Llvm)
                        pr.StartInfo = new($"clang{(s.EndsWith(".cpp") ? "++" : "")}", string.Join(' ', CCArgs) + $" -o {outp} -c {s}");
                    pr.Start();

                    while (!pr.HasExited)
                        ;;

                    if (pr.ExitCode != 0)
                    {    
                        Console.WriteLine("[FAIL] Compilation failed. Please check the error(s) shown above.");
                        return 1;
                    }

                    CompiledSourceFiles.Add(s);
                }
            }
            else if (s.EndsWith(".asm"))
            {
                outp = s.Replace(".asm", "-asm.o").Replace(Constants.SourceDirectory, Constants.ObjectDirectoryStart + a);
                
                if (a == Architecture.RiscV64 || a == Architecture.AArch64)
                    cc = false;

                if (cc)
                    cc = FileLineCheck(content[0], ";", a);

                if (cc)
                {
                    string elftype = "";
                    if (a == Architecture.x86_64)
                        elftype = "elf64";
                    else if (a == Architecture.i686)
                        elftype = "elf32";

                    Console.WriteLine($"[ASM] {s} => {outp}");
                    var pr = Process.Start("nasm", $"{s} -f{elftype} -o {outp}");
                    while (!pr.HasExited)
                        ;;

                    if (pr.ExitCode != 0)
                    {    
                        Console.WriteLine("[FAIL] Compilation failed. Please check the error(s) shown above.");
                        return 1;
                    }

                    CompiledSourceFiles.Add(s);
                }
            }
        }

        ///// Link the kernel

        string linkScriptPath = Path.Combine(Constants.TempDirectory, "lnk-" + a + ".ld");
        string outputKernelPath = "";
        
        outputKernelPath = $"kernel-{a.ToString().ToLower()}.elf";

        // Download link script
        if (!File.Exists(linkScriptPath))
        {
            HttpClient hc = new();
            FileStream s = File.Create(linkScriptPath);
            string linkScriptUrl = $"https://raw.githubusercontent.com/limine-bootloader/limine-c-template-portable/trunk/kernel/linker-{a.ToString().ToLower()}.ld";
            if (a == Architecture.i686)
                linkScriptUrl = "https://raw.githubusercontent.com/limine-bootloader/limine/v6.x-branch/test/multiboot2.ld";
            Console.WriteLine($"[LD] Downloading '{linkScriptUrl}', necessary to link the kernel.");
            var downloadedLinkScript = hc.GetStreamAsync(linkScriptUrl);
            downloadedLinkScript.Result.CopyTo(s);
            s.Close();

            var text = File.ReadAllText(linkScriptPath);
            text = text.Replace("_start", "prestart");
            File.WriteAllText(linkScriptPath, text);
        }

        List<string> ldArgs = new();
        if (t == Toolchain.Llvm)
        {
            // thanks to llvm for not applying the same templates to the -m args!!!!!!
            if (a == Architecture.x86_64)
                ldArgs.Add("-m elf_x86_64");
            else if (a == Architecture.AArch64)
                ldArgs.Add("-m aarch64elf");
            else if (a == Architecture.i686)
                ldArgs.Add("-m elf_i386");
            else if (a == Architecture.RiscV64)
                ldArgs.Add("-m elf64lriscv");
        }
        ldArgs.Add("-nostdlib");
        ldArgs.Add("-static");
        ldArgs.Add("-z max-page-size=0x1000");
        ldArgs.Add(string.Join(' ', IOUtils.GetFilesRecursivelyInDirectory(Constants.ObjectDirectoryStart + a, new string[] { ".o" })));
        
        if (Constants.OutputDirectory.Contains(' '))
            ldArgs.Add($" -o \"{Path.Combine(Constants.OutputDirectory, outputKernelPath)}\"");
        else
            ldArgs.Add($" -o {Path.Combine(Constants.OutputDirectory, outputKernelPath)}");

        if (linkScriptPath.Contains(' '))
            ldArgs.Add($"-T \"{linkScriptPath}\"");
        else
            ldArgs.Add($"-T {linkScriptPath}");

        Console.WriteLine("[LD] " + outputKernelPath);

        //Console.WriteLine(string.Join(' ', ldArgs));

        Process p = null;

        if (t == Toolchain.Gnu)
            p = Process.Start($"{a.ToString().ToLower()}-elf-ld", string.Join(' ', ldArgs));
        else if (t == Toolchain.Llvm)
            p = Process.Start($"ld.lld.exe", string.Join(' ', ldArgs));

        while (!p.HasExited)
            ;;
        
        if (p.ExitCode != 0)
            Console.WriteLine("[FAIL] Compilation failed. Please check the error(s) shown above.");

        ///// Finish by copying the source code to the temp folder
        Console.WriteLine("[POST] Copying source tree to temp");

        foreach (string d in Directory.GetDirectories(Constants.SourceDirectory, "*", SearchOption.AllDirectories))
        {
            string copy = d.Replace(Constants.SourceDirectory, Constants.TempSourceTree + $"-{a}");
            if (!Directory.Exists(copy))
                Directory.CreateDirectory(copy);
        }

        foreach (string s in IOUtils.GetFilesRecursivelyInDirectory(Constants.SourceDirectory, new string[] { ".c", ".cpp", ".asm", ".rs", ".s" }))
        {
            File.Copy(s, s.Replace(Constants.SourceDirectory, Constants.TempSourceTree + $"-{a}"), true);
        }

        return 0;
    }
}