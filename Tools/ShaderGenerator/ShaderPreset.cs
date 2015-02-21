using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml;
using System.Xml.Serialization;

namespace ShaderGenerator
{
    [XmlRoot("ShaderPresets")]
    public class ShaderPresetsConfig
    {
        [XmlElement("Preset")]
        public List<ShaderPreset> Presets { get; set; } 
    }

    public class ShaderPreset
    {
        public string PresetName { get; set; }
        public string OutputName { get; set; }

        public string Model
        {
            get { return _Model.ToString(); }
            set
            {
                ShaderModel model;
                Enum.TryParse(value, out model);
                _Model = model;
            }
        }

        public string VSInputs
        {
            get { return string.Join(",", _VSInputs); }
            set
            {
                _VSInputs = new List<VSInput>();
                foreach (var input in value.Split(",".ToCharArray(), StringSplitOptions.RemoveEmptyEntries))
                {
                    VSInput vsInput;
                    Enum.TryParse(input, out vsInput);
                    _VSInputs.Add(vsInput);
                }
            }
        }

        public string PSInputs
        {
            get { return string.Join(",", _PSInputs); }
            set
            {
                _PSInputs = new List<PSInput>();
                foreach (var input in value.Split(",".ToCharArray(), StringSplitOptions.RemoveEmptyEntries))
                {
                    PSInput psInput;
                    Enum.TryParse(input, out psInput);
                    _PSInputs.Add(psInput);
                }
            }
        }

        public string ConstantBuffers
        {
            get { return string.Join(",", _ConstantBuffers); }
            set
            {
                _ConstantBuffers = new List<ConstantBuffer>();
                foreach (var input in value.Split(",".ToCharArray(), StringSplitOptions.RemoveEmptyEntries))
                {
                    ConstantBuffer cBuffer;
                    Enum.TryParse(input, out cBuffer);
                    _ConstantBuffers.Add(cBuffer);
                }
            }
        }

        public string PSOutputs
        {
            get { return string.Join(",", _PSOutputs); }
            set
            {
                _PSOutputs = new List<PSOutput>();
                foreach (var input in value.Split(",".ToCharArray(), StringSplitOptions.RemoveEmptyEntries))
                {
                    PSOutput psOutput;
                    Enum.TryParse(input, out psOutput);
                    _PSOutputs.Add(psOutput);
                }
            }
        }

        public string PSTransforms
        {
            get { return string.Join(",", _PSTransforms); }
            set
            {
                _PSTransforms = new List<PSTransform>();
                foreach (var input in value.Split(",".ToCharArray(), StringSplitOptions.RemoveEmptyEntries))
                {
                    PSTransform psTransform;
                    Enum.TryParse(input, out psTransform);
                    _PSTransforms.Add(psTransform);
                }
            }
        }

        private ShaderModel _Model { get; set; }
        private List<VSInput> _VSInputs { get; set; }
        private List<PSInput> _PSInputs { get; set; }
        private List<ConstantBuffer> _ConstantBuffers { get; set; }
        private List<PSOutput> _PSOutputs { get; set; }
        private List<PSTransform> _PSTransforms { get; set; }

        public List<VSInput> GetVsInputs()
        {
            return _VSInputs;
        }

        public List<PSInput> GetPsInputs()
        {
            return _PSInputs;
        }

        public List<ConstantBuffer> GetConstantBuffers()
        {
            return _ConstantBuffers;
        }

        public List<PSOutput> GetPsOutputs()
        {
            return _PSOutputs;
        }
        public List<PSTransform> GetPsTransforms()
        {
            return _PSTransforms;
        }
    }

    public enum ShaderModel
    {
        vs_5_0,
        vs_4_0,
        ps_5_0,
        ps_4_0
    }

    public enum VSInput
    {
        color,
        tex,
        normal
    }

    public enum PSInput
    {
        viewDirection,
    }

    public enum ConstantBuffer
    {
        camera,
        textureIn
    }

    public enum PSOutput
    {
        float4,
        gBuffer
    }

    public enum PSTransform
    {
        texSample,
        colorDirect
    }
}
