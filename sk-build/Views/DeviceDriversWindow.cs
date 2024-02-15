using Terminal.Gui;

namespace SipaaKernel.Builder.Views;

public class DeviceDriversWindow : Window
{
    public static int Tag = 0;

    public DeviceDriversWindow()
    {
        Tag = Random.Shared.Next(999999999);
        Title = "DevDrivers|Press ESC to close";
        BorderStyle = LineStyle.Rounded;
        this.ColorScheme = new ColorScheme(new Terminal.Gui.Attribute(ColorName.White, ColorName.Black));
        KeyPressed += (s,e) => 
        {
            if (e.KeyEvent.Key == Key.Esc && Application.Top.Focused == this)
                Application.Top.Remove(this);
        };

        var enablePciInKernel = new CheckBox("Enable PCI compilation", SKConfig.Current.EnablePCIC);
        enablePciInKernel.Toggled += (s,e) => { SKConfig.Current.EnablePCIC = (bool)e.NewValue; };
        Add(enablePciInKernel);
    }
}