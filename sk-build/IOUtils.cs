namespace SipaaKernel.Builder;

public class IOUtils
{
    public static List<string> GetFilesRecursivelyInDirectory(string dir, string[] allowedExtensions)
    {
        List<string> FilePaths = new();
        var fileList = Directory.GetFiles(dir, "*.*", SearchOption.AllDirectories)
                             .Where(file => allowedExtensions.Contains(Path.GetExtension(file)))
                             .ToArray();
        foreach (string f in fileList)
        {
            FilePaths.Add(f);
        }
        return FilePaths;
    }
}