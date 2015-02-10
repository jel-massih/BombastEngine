using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Text;

namespace ShaderGenerator
{
    public class EffectCompiler
    {
        public static bool TryCompile(string shaderCode, string shaderModelType, string entrypoint, out string error)
        {
            string path = Path.GetTempFileName();

            using (FileStream fs = new FileStream(path, FileMode.Create))
            {
                byte[] dataBytes = Encoding.ASCII.GetBytes(shaderCode);
                fs.Write(dataBytes, 0, dataBytes.Length);
            }

            var fxcPath = Path.Combine(new Uri(Path.GetDirectoryName(Assembly.GetEntryAssembly().Location)).AbsolutePath, @"fxc.exe");
            if (!File.Exists(fxcPath))
            {
                error = "Effect Copiler fxc Not Found";
                return false;
            }

            ProcessStartInfo processInfo = new ProcessStartInfo(fxcPath);
            processInfo.CreateNoWindow = true;
            processInfo.UseShellExecute = false;
            processInfo.RedirectStandardError = true;
            processInfo.Arguments = string.Format("/T {1} /E {2} /Fo\"{0}.obj\" \"{0}\"", path, shaderModelType, entrypoint);

            error = "";

            using (Process p = Process.Start(processInfo))
            {
                StreamReader sr = p.StandardError;
                error = sr.ReadToEnd().Replace(path, "Line ");

                if (!p.WaitForExit(5000))
                {
                    error = "Compile Timeout";
                    return false;
                }
            }

            if (File.Exists(path))
            {
                File.Delete(path);
            }

            if (File.Exists(path + ".obj"))
            {
                File.Delete(path + ".obj");
            }

            return error == "";
        }
    }
}
