using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace BombastEditor
{
    public class PathUtils
    {
        static public string NormalizeFilepath(string filepath)
        {
            string result = System.IO.Path.GetFullPath(filepath).ToLowerInvariant();

            result = result.TrimEnd(new[] { '\\' });

            return result;
        }

        public static string GetRelativePath(string rootPath, string fullPath)
        {
            rootPath = NormalizeFilepath(rootPath);
            fullPath = NormalizeFilepath(fullPath);

            if (!fullPath.StartsWith(rootPath))
                throw new Exception("Could not find rootPath in fullPath when calculating relative path.");

            return "." + fullPath.Substring(rootPath.Length);
        }
    }

    class XPathUtility
    {
        static int GetNodePosition(XmlNode child)
        {
            int count = 1;
            for (int i = 0; i < child.ParentNode.ChildNodes.Count; i++)
            {
                if (child.ParentNode.ChildNodes[i] == child)
                {
                    return count;
                }
                if (child.ParentNode.ChildNodes[i].Name == child.Name)
                {
                    ++count;
                }
            }
            throw new InvalidOperationException("Child node not found in its parent's ChildNodes property.");
        }

        public static string GetXPathToNode(XmlNode node)
        {
            if (node.NodeType == XmlNodeType.Attribute)
            {
                return String.Format("{0}/@{1}", GetXPathToNode(((XmlAttribute)node).OwnerElement), "*" );
            }

            if (node.ParentNode == null)
            {
                return "";
            }

            return String.Format("{0}/{1}[{2}]", GetXPathToNode(node.ParentNode), "*", GetNodePosition(node));
        }
    }
}
