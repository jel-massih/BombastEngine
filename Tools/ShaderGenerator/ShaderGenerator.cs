using System;
using System.Collections;
using System.Collections.Generic;
using System.Configuration;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Resources;
using System.Windows.Forms;
using System.Xml.Serialization;
using MetroFramework.Forms;
using ShaderGenerator.Config;

namespace ShaderGenerator
{
    public partial class ShaderGenerator : MetroForm
    {
        private ShaderPresetsConfig _presetsConfig;
        private Dictionary<string, string> _shaderModelDictionary; 

        public ShaderGenerator()
        {
            InitializeComponent();

            InitializeShaderModelSelect();
            InitializeShaderPresetSelect();
        }

        private void InitializeShaderModelSelect()
        {
            shaderModelSelect.Items.Clear();
            _shaderModelDictionary = new Dictionary<string, string>();
            ResourceSet resourceSet = ShaderProfiles.ResourceManager.GetResourceSet(CultureInfo.CurrentUICulture, true, true);
            foreach (DictionaryEntry entry in resourceSet)
            {
                //dict.Add(entry.Key.ToString(), entry.Value.ToString());
            }
            _shaderModelDictionary.Add("vs_4_0", "Vertex Shader 4.0");
            _shaderModelDictionary.Add("ps_4_0", "Pixel Shader 4.0");

            shaderModelSelect.DataSource = new BindingSource(_shaderModelDictionary, null);
            shaderModelSelect.DisplayMember = "Value";
            shaderModelSelect.ValueMember = "Key";
        }

        private void InitializeShaderPresetSelect()
        {
            shaderPresetsSelect.Items.Clear();
            var list = new List<string>() {""};

            var serializer = new XmlSerializer(typeof(ShaderPresetsConfig));
            var stream = new FileStream("../../ShaderGenerator/Config/ShaderPresets.config", FileMode.Open);
            _presetsConfig = (ShaderPresetsConfig)serializer.Deserialize(stream);

            foreach (var preset in _presetsConfig.Presets)
            {
                list.Add(preset.PresetName);
            }

            shaderPresetsSelect.DataSource = new BindingSource(list, null);
        }

        private void generateBtn_Click(object sender, System.EventArgs e)
        {
            var shaderDefines = GetShaderDefines();
            var shaderName = shaderNameInput.Text;
            var shaderModel = shaderModelSelect.SelectedItem.GetType().GetProperty("Key").GetValue(shaderModelSelect.SelectedItem).ToString();
            var entryPoint = "VSMain";

            if (string.IsNullOrEmpty(shaderName))
            {
                generationStatusLabel.ForeColor = Color.DarkRed;
                generationStatusLabel.Text = "Please Enter a Shader Name";
                return;
            }

            if (shaderModel == "ps_4_0")
            {
                entryPoint = "PSMain";
            }

            var generatedShaderCode = GenerateShaderCode(shaderDefines);

            System.Diagnostics.Debug.WriteLine(generatedShaderCode);
            currentEventLabel.Text = "Generating Shader: " + shaderName + " with model: " + shaderModel;

            string error;
            var compileSuccess = EffectCompiler.TryCompile(generatedShaderCode, shaderModel, entryPoint, ConfigurationManager.AppSettings["OutputPath"] + shaderName, out error);

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
            foreach (var define in shaderDefines)
            {
                code += "#define " + define + " \n";
            }

            code += string.Format("#include \"{0}\"", ConfigurationManager.AppSettings["UberShaderPath"]);
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

            if (PIXEL_OUT_FLOAT4.Checked)
                shaderDefines.Add("PIXEL_OUT_FLOAT4");

            if (PIXEL_OUT_GBUFFER.Checked)
                shaderDefines.Add("PIXEL_OUT_GBUFFER");

            if (USE_ALBEDO_TEXTURE.Checked)
                shaderDefines.Add("USE_ALBEDO_TEXTURE");

            if (USE_NORMAL_MAP.Checked)
                shaderDefines.Add("USE_NORMAL_MAP");

            if (CALC_LIGHTING.Checked)
                shaderDefines.Add("CALC_LIGHTING");

            return shaderDefines;
        }

        private void shaderPresetsSelect_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            if (string.IsNullOrEmpty(shaderPresetsSelect.SelectedItem.ToString())) { return; }

            var selectedPreset = _presetsConfig.Presets.FirstOrDefault(r => r.PresetName == shaderPresetsSelect.SelectedItem.ToString());
            if (selectedPreset != null)
            {
                shaderNameInput.Text = selectedPreset.OutputName;
                shaderModelSelect.SelectedIndex = Array.IndexOf(_shaderModelDictionary.Keys.ToArray(), selectedPreset.Model);

                VERT_INPUT_COLOR.Checked = selectedPreset.GetVsInputs().Contains(VSInput.color);
                VERT_INPUT_TEX.Checked = selectedPreset.GetVsInputs().Contains(VSInput.tex);
                VERT_INPUT_NORMAL.Checked = selectedPreset.GetVsInputs().Contains(VSInput.normal);

                PIXEL_INPUT_VIEWDIR.Checked = selectedPreset.GetPsInputs().Contains(PSInput.viewDirection);

                PIXEL_OUT_FLOAT4.Checked = selectedPreset.GetPsOutputs().Contains(PSOutput.float4);
                PIXEL_OUT_GBUFFER.Checked = selectedPreset.GetPsOutputs().Contains(PSOutput.gBuffer);

                USE_NORMAL_MAP.Checked = selectedPreset.GetPsTransforms().Contains(PSTransform.normalMap);
                USE_ALBEDO_TEXTURE.Checked = selectedPreset.GetPsTransforms().Contains(PSTransform.albedoTexture);
                CALC_LIGHTING.Checked = selectedPreset.GetPsTransforms().Contains(PSTransform.calcLighting);
            }
        }
    }
}
