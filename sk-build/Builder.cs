using System.Diagnostics;
using System.Reflection;
using SipaaKernel.Builder.Model;

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

    private static Target? GetTarget(string name)
    {
        return Project.CurrentProject.Targets.Select(tgt => tgt.Name == name ? tgt : null).First();
    }

    public static int Build(BuildOptions opt)
    {
        Target? t = GetTarget(opt.Target);
        if (t == null)
            return 1;

        List<string> source = IOUtils.GetFilesRecursivelyInDirectory(Constants.SourceDirectory, new string[] { ".c", ".cpp", ".rs", ".s", ".asm" });
        bool cc = true;
        string outp = "";
        List<string> CompiledSourceFiles = new();

        ///// Prepare the build
        PrepDirs(t.Architecture);
        AddConfigDefinesToArguments(t.CCFlags.ToList());

        ///// Build the source code
        foreach (string s in source)
        {
            cc = true;

            if (File.Exists(s.Replace(Constants.SourceDirectory, Constants.TempSourceTree + "-" + t.Architecture)))
            {
                string savedText = File.ReadAllText(s.Replace(Constants.SourceDirectory, Constants.TempSourceTree + "-" + t.Architecture));
                string currentText = File.ReadAllText(s);

                if (savedText == currentText)
                    cc = false;
            }

            string[] content = File.ReadAllLines(s);

            if (s.EndsWith(".c") || s.EndsWith(".cpp"))
            {
                outp = s.Replace(s.EndsWith("pp") ? ".cpp" : ".c", ".o").Replace(Constants.SourceDirectory, Constants.ObjectDirectoryStart + t.Architecture);

                if (cc && content.Length > 0)
                    cc = FileLineCheck(content[0], "// ", t.Architecture);

                if (cc)
                {
                    Console.WriteLine($"[{(s.EndsWith("pp") ? "CXX" : "CC")}] {s} => {outp}");
                    
                    Process pr = new();
                    pr.StartInfo = new(s.EndsWith("pp") ? t.CXX : t.CC, string.Join(' ', s.EndsWith("pp") ? t.CXXFlags : t.CCFlags).Replace("%obj%", outp).Replace("%src%", s));
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
                outp = s.Replace(".asm", "-asm.o").Replace(Constants.SourceDirectory, Constants.ObjectDirectoryStart + t.Architecture);

                if (cc)
                    cc = FileLineCheck(content[0], ";", t.Architecture);

                if (cc)
                {
                    Console.WriteLine($"[ASM] {s} => {outp}");
                    var pr = Process.Start(t.ASM, string.Join(' ', t.ASMFlags).Replace("%obj%", outp).Replace("%src%", s));
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

        string outputKernelPath = t.OutputBinary.Replace("%arch%", t.Architecture.ToString());

        Console.WriteLine("[LD] " + outputKernelPath);

        //Console.WriteLine(string.Join(' ', ldArgs));

        Process p = null;
        p = Process.Start(t.LD, string.Join(' ', t.LDFlags).Replace("%lds%", t.LDScript).Replace("%objs%", string.Join(' ', IOUtils.GetFilesRecursivelyInDirectory(Constants.ObjectDirectoryStart + t.Architecture, new string[] { ".o" }))).Replace("%bin%", string.Join(' ', t.OutputBinary)));
        Console.WriteLine(p.StartInfo.Arguments);
        
        while (!p.HasExited)
            ;;
        
        if (p.ExitCode != 0) 
        {
            Console.WriteLine("[FAIL] Compilation failed. Please check the error(s) shown above.");
            return 1;
        }

        ///// Finish by copying the source code to the temp folder
        Console.WriteLine("[POST] Copying source tree to temp");

        foreach (string d in Directory.GetDirectories(Constants.SourceDirectory, "*", SearchOption.AllDirectories))
        {
            string copy = d.Replace(Constants.SourceDirectory, Constants.TempSourceTree + $"-{t.Architecture}");
            if (!Directory.Exists(copy))
                Directory.CreateDirectory(copy);
        }

        foreach (string s in IOUtils.GetFilesRecursivelyInDirectory(Constants.SourceDirectory, new string[] { ".c", ".cpp", ".asm", ".rs", ".s" }))
        {
            File.Copy(s, s.Replace(Constants.SourceDirectory, Constants.TempSourceTree + $"-{t.Architecture}"), true);
        }

        return 0;
    }
}