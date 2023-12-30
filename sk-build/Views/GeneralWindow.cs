using Terminal.Gui;

namespace SipaaKernel.Builder.Views;

public class GeneralWindow : Window
{
    public static int Tag = 0;

    public GeneralWindow()
    {
        Tag = Random.Shared.Next(999999999);
        Title = "General|Press ESC to close";
        BorderStyle = LineStyle.Rounded;
        this.ColorScheme = new ColorScheme(new Terminal.Gui.Attribute(ColorName.White, ColorName.Black));
        KeyPressed += (s,e) => 
        {
            if (e.KeyEvent.Key == Key.Esc && Application.Top.Focused == this)
                Application.Top.Remove(this);
        };

        var logsConIO = new CheckBox(0, 0, "Write logs to the console", SKConfig.Current.LogsConIO);
        logsConIO.Toggled += (s,e) => { SKConfig.Current.LogsConIO = (bool)e.NewValue; };
        Add(logsConIO);

        var addCpOptsText = new TextField(0, 1, 20, "AdditionalCpOpts");
        
        Add(addCpOptsText);

        var addCpOpts = new TextView(new(0, 2, 20, 1));
        addCpOpts.Text = SKConfig.Current.AdditionalCompileOptions;
        //logsConIO.Toggled += (s,e) => { SKConfig.Current.LogsConIO = (bool)e.NewValue; };
        Add(logsConIO);
    }
}