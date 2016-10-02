using System.IO;

namespace BombastEditor
{
    public class BombastResource
    {
        public string FullFilepath { get; set; }

        public string ResourceName { get; set; }

        public static string GetResourceNameFromPath(string filepath)
        {
            //remove a leading ./ and replace slashes with dots
            return filepath.Replace(".\\", "").Replace("./", ".").Replace('\\', '.').Replace('/', '.');
        }
    }
}
