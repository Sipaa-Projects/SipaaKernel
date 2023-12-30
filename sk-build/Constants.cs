namespace SipaaKernel.Builder;

public class Constants
{
    public static readonly string ObjectDirectoryStart = "obj-";
    public static readonly string OutputDirectory = Path.Combine(Environment.CurrentDirectory, "output");
    public static readonly string SourceDirectory = Path.Combine(Environment.CurrentDirectory, "src");
    public static readonly string TempDirectory = Path.Combine(Environment.CurrentDirectory, ".skb");
    public static readonly string TempSourceTree = Path.Combine(TempDirectory, "srctree");
}