using Newtonsoft.Json;

namespace SipaaKernel.Builder.Model;

public class Target
{
    public string Name;
    public string Description;
    public string CC;
    public string CXX;
    public string LD;
    public string ASM;
    public string[] CCFlags;
    public string[] CXXFlags;
    public string[] ASMFlags;
    public string[] LDFlags;
    public string LDScript;
    public string OutputBinary;
    public Architecture Architecture;
}