using System;
using System.IO;

namespace BombastEditor
{
    public class BombastResource
    {
        public string FullFilepath { get; set; }

        public string ResourceName { get; set; }

        public BombastResourceType ResourceType { get; set; }

        public static string GetResourceNameFromPath(string filepath)
        {
            //remove a leading ./ and replace slashes with dots
            return filepath.Replace(".\\", "").Replace("./", ".").Replace('\\', '.').Replace('/', '.');
        }
    }

    public enum BombastResourceType
    {
        NONE = 0,
        PROJECT = 1, //.bmap
        LEVEL = 2, //.bmap
        ACTOR = 3 //.xml
    }

    public static class BombastResourceHelper
    {
        public static BombastResourceType GetTypeFromExtension(string extension)
        {
            switch(extension.ToLower())
            {
                case ".bproject":
                    return BombastResourceType.PROJECT;
                case ".bmap":
                    return BombastResourceType.LEVEL;
                case ".xml":
                    return BombastResourceType.ACTOR;
                default:
                    return BombastResourceType.NONE;
            }
        }

        public static string GetExtension(this BombastResourceType resourceType)
        {
            switch(resourceType)
            {
                case BombastResourceType.PROJECT:
                    return ".bproject";
                case BombastResourceType.LEVEL:
                    return ".bmap";
                case BombastResourceType.ACTOR:
                    return ".xml";
                default:
                    return null;
            }
        }

        public static BombastResource GetBombastResourceFromFilepath(string filePath, string baseFilepath = "")
        {
            var relativePath = filePath;
            if (!string.IsNullOrEmpty(baseFilepath))
            {
                relativePath = PathUtils.GetRelativePath(baseFilepath, filePath);
            }
            var resourceInfo = new BombastResource
            {
                FullFilepath = filePath,
                ResourceName = BombastResource.GetResourceNameFromPath(relativePath),
                ResourceType = GetTypeFromExtension(Path.GetExtension(filePath))
            };

            return resourceInfo;
        }
    }
}
