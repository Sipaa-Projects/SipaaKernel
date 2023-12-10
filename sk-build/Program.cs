using SipaaKernel.Builder.Views;
using Terminal.Gui;

namespace SipaaKernel.Builder;

public class MainFrame : Toplevel
{
    public MainFrame()
    {
        var introLabel = new Label()
        {
            X = 3,
            Y = 2,
            Text = "To start configuring SipaaKernel, go into the 'View' menu (ALT+V)"
        };

        var menu = new MenuBar (new MenuBarItem [] {
            new MenuBarItem ("SipaaKernel/x86 Configuration", new MenuItem [] {}),
            new MenuBarItem ("_File", new MenuItem [] {
                new MenuItem ("_Reset configuration to previous state", "", () => { 
                    SKConfig.Load();
                }),
                new MenuItem ("_New configuration", "", () => { 
                    SKConfig.Current = new();
                }),
                new MenuItem ("_Save configuration", "", () => { 
                    SKConfig.Save();
                }),
                new MenuItem ("_Save configuration as...", "", () => { 
                    var s = new SaveDialog();
                    s.AllowsMultipleSelection = false;
                    s.Title = "Save configuration as...";
                    s.Path = "/home";
                    s.FilesSelected += (sn,e) => {
                        Remove(s);
                        SKConfig.Save(Path.Join(s.Path, s.FileName));
                        MessageBox.Query("Success", "The configuration has been saved.", "OK");
                        e.Cancel = true;
                    };
                    Add(s);
                }),
                new MenuItem ("_Quit", "", () => { 
                    Application.RequestStop (); 
                })
            }),
            new MenuBarItem ("_View", new MenuItem [] {
                new MenuItem ("_Device Drivers", "", () => { 
                    Add(new DeviceDriversWindow() { X = 10, Y = 10, Width = 40, Height = 10 });
                })
            }),
        });
        Add(introLabel);
        Add(menu);
    }
}

public class Program
{
    static void Clean()
    {
        if (Directory.Exists(Path.Combine(Environment.CurrentDirectory, ".skb")))
            Directory.Delete(Path.Combine(Environment.CurrentDirectory, ".skb"), true);
        if (Directory.Exists(Path.Combine(Environment.CurrentDirectory, "obj-x86_64")))
            Directory.Delete(Path.Combine(Environment.CurrentDirectory, "obj-x86_64"), true);
        if (Directory.Exists(Path.Combine(Environment.CurrentDirectory, "obj-AArch64")))
            Directory.Delete(Path.Combine(Environment.CurrentDirectory, "obj-AArch64"), true);
        if (Directory.Exists(Path.Combine(Environment.CurrentDirectory, "obj-RiscV64")))
            Directory.Delete(Path.Combine(Environment.CurrentDirectory, "obj-RiscV64"), true);
        if (Directory.Exists(Path.Combine(Environment.CurrentDirectory, "output")))
            Directory.Delete(Path.Combine(Environment.CurrentDirectory, "output"), true);
    }
    static void Config()
    {
        Application.Run<MainFrame>();
        Application.Shutdown();
    }

    static void Help()
    {
        Console.WriteLine("sk-build - the SipaaKernel build & configuration utility.");
        Console.WriteLine();
        Console.WriteLine("Available commands: ");
        Console.WriteLine("  - help : Show this message (the help message)");
        Console.WriteLine("  - version : Show the version of 'sk-build'");
        Console.WriteLine("  - config : Configure SipaaKernel");
    }

    static void Version()
    {
        Console.WriteLine("sk-build - the SipaaKernel build & configuration utility.");
        Console.WriteLine("Copyight (C) 2023-present Sipaa Projects.");
        Console.WriteLine("This project is part of SipaaKernel & licensed under the MIT license.");
        Console.WriteLine();
        Console.WriteLine("Version 0.1 (Beta)");
    }

    static void Main(string[] args)
    {
        try
        {
            if (args.Length == 0)
            {
                Console.WriteLine("You must specify at least one argument. Please check help for details : 'sk-build help");
                return;
            }
            else
            {
                if (!SKConfig.Load())
                {
                    Console.WriteLine("Cannot load configuration properly");
                    Environment.Exit(1);
                }
                if (args[0] == "help")
                    Help();
                else if (args[0] == "version")
                    Version();
                else if (args[0] == "config")
                    Config();
                else if (args[0] == "build")
                    Builder.Build(Architecture.x86_64);
                else if (args[0] == "build-aarch64")
                    Builder.Build(Architecture.AArch64);
                else if (args[0] == "build-riscv64")
                    Builder.Build(Architecture.RiscV64);
                else if (args[0] == "clean")
                    Clean();
            }
        }
        catch (Exception e)
        {
            Application.Shutdown();
            Console.WriteLine(e);
        }
    }
}