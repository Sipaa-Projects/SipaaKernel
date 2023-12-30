using Newtonsoft.Json;

namespace SipaaKernel.Builder;

public class SKConfig
{
    [JsonIgnore]
    public static SKConfig? Current { get; set; } = null;

    public static bool Save()
    {
        try
        {
            if (Current != null) 
            {
                string json = JsonConvert.SerializeObject(Current);
                File.WriteAllText(Path.Join(Environment.CurrentDirectory, "skconfig.json"), json);
            }
            return true;
        }
        catch 
        {
            return false;
        }
    }

    public static bool Save(string to)
    {
        try
        {
            if (Current != null) 
            {
                string json = JsonConvert.SerializeObject(Current, Formatting.Indented);
                File.WriteAllText(to, json);
            }
            return true;
        }
        catch 
        {
            return false;
        }
    }

    public static bool Load()
    {
        try
        {
            if (!File.Exists(Path.Join(Environment.CurrentDirectory, "skconfig.json")))
            {
                Current = new();
                return true;
            }
            Current = JsonConvert.DeserializeObject<SKConfig>(File.ReadAllText(Path.Join(Environment.CurrentDirectory, "skconfig.json")));
            return true;
        }
        catch 
        {
            return false;
        }
    }

    public bool EnablePCIC { get; set; } = false;
    public bool LogsConIO { get; set; } = false;
    public string AdditionalCompileOptions { get; set; } = "";
}
