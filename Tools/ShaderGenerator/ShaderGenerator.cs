using System.Collections;
using System.Collections.Generic;
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
                dict.Add(entry.Key.ToString(), entry.Value.ToString());
            }

            shaderModelSelect.DataSource = new BindingSource(dict, null);
            shaderModelSelect.DisplayMember = "Value";
            shaderModelSelect.ValueMember = "Key";
        }
    }
}
