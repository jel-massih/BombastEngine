using System.Xml.Serialization;

namespace BombastEditor.BombastResourceTypes
{
    class XmlColor
    {
        [XmlAttribute(AttributeName = "r")]
        public float R { get; set; }

        [XmlAttribute(AttributeName = "g")]
        public float G { get; set; }

        [XmlAttribute(AttributeName = "b")]
        public float B { get; set; }

        [XmlAttribute(AttributeName = "a")]
        public float A { get; set; }
    }

    class XmlVector3
    {
        [XmlAttribute(AttributeName = "x")]
        public float X { get; set; }

        [XmlAttribute(AttributeName = "y")]
        public float Y { get; set; }

        [XmlAttribute(AttributeName = "z")]
        public float Z { get; set; }
    }
}
