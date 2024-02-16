using Newtonsoft.Json;

namespace SipaaKernel.Builder.Model;

public class Project
{
    [JsonIgnore]
    public static Project CurrentProject;

    public string Name;
    public string Description;
    public string Copyright;
    public string License;
    public Target[] Targets;
}