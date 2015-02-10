using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Globalization;
using System.Resources;
using System.Windows.Forms;
using MetroFramework.Forms;
using ShaderGenerator.Config;

namespace ShaderGenerator
{
    public partial class ShaderGenerator : MetroForm
    {
        public ShaderGenerator()
        {
            InitializeComponent();

            InitializeShaderModelSelect();
        }

        private void InitializeShaderModelSelect()
        {
            shaderModelSelect.Items.Clear();
            var dict = new Dictionary<string, string>();
            ResourceSet resourceSet = ShaderProfiles.ResourceManager.GetResourceSet(CultureInfo.CurrentUICulture, true, true);
            foreach (DictionaryEntry entry in resourceSet)
            {
                //dict.Add(entry.Key.ToString(), entry.Value.ToString());
            }
            dict.Add("vs_4_0", "Vertex Shader 4.0");
            dict.Add("ps_4_0", "Pixel Shader 4.0");

            shaderModelSelect.DataSource = new BindingSource(dict, null);
            shaderModelSelect.DisplayMember = "Value";
            shaderModelSelect.ValueMember = "Key";
        }

        private void generateBtn_Click(object sender, System.EventArgs e)
        {
            var shaderDefines = GetShaderDefines();
            var shaderName = shaderNameInput.Text;
            var shaderModel = shaderModelSelect.SelectedItem.GetType().GetProperty("Key").ToString();
            var entryPoint = "VSMain";

            if (shaderModel == "ps_4_0")
            {
                entryPoint = "PSMain";
            }

            var generatedShaderCode = GenerateShaderCode(shaderDefines);

            string error;
            var compileSuccess = EffectCompiler.TryCompile(generatedShaderCode, shaderModel, entryPoint, out error);

            if (compileSuccess)
            {
                generationStatusLabel.ForeColor = Color.Green;
                generationStatusLabel.Text = "Compilation Successful!";
            }
            else
            {
                generationStatusLabel.ForeColor = Color.DarkRed;
                generationStatusLabel.Text = "Compile Error: " + error;
            }
        }

        private string GenerateShaderCode(List<string> shaderDefines)
        {
            var code = "";

            return code;
        }

        private List<string> GetShaderDefines()
        {
            var shaderDefines = new List<string>();
            if (VERT_INPUT_COLOR.Checked)
                shaderDefines.Add("VERT_INPUT_COLOR");

            if (VERT_INPUT_TEX.Checked)
                shaderDefines.Add("VERT_INPUT_TEX");

            if (VERT_INPUT_NORMAL.Checked)
                shaderDefines.Add("VERT_INPUT_NORMAL");

            if (PIXEL_INPUT_VIEWDIR.Checked)
                shaderDefines.Add("PIXEL_INPUT_VIEWDIR");

            if (ENABLE_TEXTURE_INPUT.Checked)
                shaderDefines.Add("ENABLE_TEXTURE_INPUT");

            if (ENABLE_CAMERA_BUFFER.Checked)
                shaderDefines.Add("ENABLE_CAMERA_BUFFER");

            if (PIXEL_OUT_FLOAT4.Checked)
                shaderDefines.Add("PIXEL_OUT_FLOAT4");

            if (PIXEL_OUT_GBUFFER.Checked)
                shaderDefines.Add("PIXEL_OUT_GBUFFER");

            if (PIXEL_COLOR_DIRECT.Checked)
                shaderDefines.Add("PIXEL_COLOR_DIRECT");

            if (PIXEL_TEX_SAMPLE.Checked)
                shaderDefines.Add("PIXEL_TEX_SAMPLE");

            return shaderDefines;
        }
    }
}
