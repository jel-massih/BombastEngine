using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BombastEditor
{
    public static class BombastHelper
    {
        public static bool CreateProject(string projectPath, string projectName)
        {
            //Create project
            Directory.CreateDirectory(projectPath);
            var stream = File.CreateText(Path.Combine(projectPath, projectName + ".bproject"));
            stream.WriteLine("Test Project File");
            stream.Close();

            //Create Assets Folder
            Directory.CreateDirectory(Path.Combine(projectPath, GlobalSettings.AssetsFolderName));

            return true;
        }
    }
}
