using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using System.Xml;

namespace BombastEditor
{
    public partial class BombastEditorForm : Form
    {
        class ActorComponentEditor
        {
            Dictionary<string, XmlNode> m_componentsByName;
            XmlDocument m_selectedActorComponents;
            int m_selectedActorId;
            XmlNode m_selectedActorXml;
            string m_assetsDirectory;

            const int g_labelColumnWidth = 160;
            int m_lineSpacing;
            Panel m_panel;

            public ActorComponentEditor(Panel panel, string assetsDirectory)
            {
                m_componentsByName = new Dictionary<string, XmlNode>();

                m_panel = panel;
                m_lineSpacing = m_panel.Font.Height * 2;

                m_assetsDirectory = assetsDirectory;

                XmlDocument componentsXml = new XmlDocument();
                componentsXml.Load("Resources/ActorComponents.xml");

                XmlElement root = componentsXml.DocumentElement;
                XmlNodeList components = root.SelectNodes("child::*");
                foreach(XmlNode component in components)
                {
                    m_componentsByName[component.Attributes["name"].Value] = component;
                }
            }

            internal unsafe void ShowActorComponents(int selectedActorId, XmlNode actorXml)
            {
                m_selectedActorId = selectedActorId;
                m_selectedActorXml = actorXml;

                m_selectedActorComponents = new XmlDocument();
                XmlNode editorComponents = m_selectedActorComponents.CreateElement("Actor");
                m_selectedActorComponents.AppendChild(editorComponents);

                m_panel.Controls.Clear();

                XmlNodeList actorValueComponents = m_selectedActorXml.SelectNodes("child::*");
                int lineNum = 0;

                foreach (XmlNode actorValueComponent in actorValueComponents)
                {
                    XmlNode sourceEditorComponent = m_componentsByName[actorValueComponent.Name];
                    XmlDocument ownerDoc = editorComponents.OwnerDocument;
                    XmlNode editorComponent = ownerDoc.ImportNode(sourceEditorComponent, true);
                    editorComponents.AppendChild(editorComponent);
                    lineNum = AddComponentUI(actorValueComponent, editorComponent, lineNum);
                }
            }

            private int AddComponentUI(XmlNode actorComponentValues, XmlNode editorComponentValues, int lineNum)
            {
                var componentName = actorComponentValues.Name.ToString();
                var componentXpath = XPathUtility.GetXPathToNode(actorComponentValues);

                try
                {
                    AddElementLabel(componentName, lineNum);
                    lineNum++;

                    foreach (XmlNode inputField in editorComponentValues)
                    {
                        string xPath = XPathUtility.GetXPathToNode(inputField);
                        string elementName = inputField.Attributes["name"].Value;
                        string elementType = inputField.Attributes["type"].Value;

                        XmlNode actorValues = null;
                        
                        foreach(XmlNode childNode in actorComponentValues.ChildNodes)
                        {
                            if(childNode.Name == elementName)
                            {
                                actorValues = childNode;
                                break;
                            }
                        }

                        AddElementLabel("   " + elementName, lineNum);
                        switch (elementType)
                        {
                            case "int":
                            case "float":
                                string format = (elementType == "int") ? "0" : "0.000";
                                AddNum(actorValues, xPath, format, lineNum);
                                lineNum++;
                                break;
                            case "Vec3":
                                AddVec3(actorValues, xPath, lineNum);
                                lineNum++;
                                break;
                            default:
                                AddElementLabel("   " + elementName + ": " + elementType + " (unknown Type)", lineNum);
                                lineNum++;
                                break;
                        }
                    }
                }
                catch (Exception e)
                {
                    MessageBox.Show("Error in ComponentName " + componentName + "\n" + e.ToString());
                }

                return lineNum;
            }

            private XmlNode FindEditorElementFromXPath(string xpath)
            {
                XmlNode root = m_selectedActorComponents.FirstChild;
                XmlNodeList nodeList = root.SelectNodes(xpath);
                return nodeList[0];
            }

            private XmlNode FindActorElementFromXPath(string xpath)
            {
                XmlNodeList nodeList = m_selectedActorXml.SelectNodes(xpath);
                return nodeList[0];
            }

            public void AddElementLabel(string labelText, int lineNum)
            {
                Label label = new Label();
                Point location = new Point(0, lineNum * m_lineSpacing);
                label.Location = location;
                label.Text = labelText;
                label.AutoSize = true; 
                m_panel.Controls.Add(label);
            }

            private void AddNum(XmlNode actorValues, string xPath, string format, int lineNum)
            {
                const int boxWidth = 60;

                TextBox textbox = new TextBox();
                Point location = new Point(g_labelColumnWidth, lineNum * m_lineSpacing);
                textbox.Name = xPath;

                string actorValue = actorValues.FirstChild.Value;
                textbox.Text = actorValue;
                textbox.Location = location;
                textbox.TextAlign = HorizontalAlignment.Right;
                textbox.Leave += new EventHandler(NumElementChanged);
                textbox.Width = boxWidth;

                m_panel.Controls.Add(textbox);
            }

            private void NumElementChanged(object sender, EventArgs e)
            {
                try
                {
                    TextBox textBox = (TextBox)sender;
                    string xPath = textBox.Name;
                    string newValue = textBox.Text;

                    XmlDocument xmlDoc = new XmlDocument();
                    XmlElement xmlActor = xmlDoc.CreateElement("Actor");
                    xmlDoc.AppendChild(xmlActor);

                    XmlAttribute xmlActorId = xmlDoc.CreateAttribute("id");
                    xmlActorId.InnerText = m_selectedActorId.ToString();
                    xmlActor.Attributes.Append(xmlActorId);

                    XmlNode elementNode;
                    XmlNode node = FindActorElementFromXPath(xPath);
                    if (node.ParentNode == null)
                    {
                        XmlAttribute attribute = (XmlAttribute)node;
                        elementNode = attribute.OwnerElement;

                        attribute.Value = newValue;
                    }
                    else
                    {
                        elementNode = node;
                        elementNode.InnerText = newValue;
                    }

                    XmlNode componentNode = elementNode.ParentNode;

                    string componentName = componentNode.Name;
                    string elementName = elementNode.Name;

                    XmlElement xmlComponent = xmlDoc.CreateElement(componentName);
                    xmlActor.AppendChild(xmlComponent);

                    XmlNode newElementNode = xmlDoc.ImportNode(elementNode, true);

                    xmlComponent.AppendChild(newElementNode);

                    NativeMethods.ModifyActor(xmlDoc.InnerXml);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
                }
            }

            public void AddVec3(XmlNode actorValues, string xPath, int lineNum)
            {
                const int horizontalSpacing = 10;
                const int boxWidth = 60;

                XmlNode fieldsElement = FindEditorElementFromXPath(xPath);
                string fieldNames = fieldsElement.Attributes["fieldNames"].Value;
                string[] fields = fieldNames.Split(',');

                for (int i = 0; i < 3; ++i)
                {
                    TextBox textBox = new TextBox();
                    Point location = new Point(g_labelColumnWidth + (i * boxWidth + horizontalSpacing), lineNum * m_lineSpacing);
                    textBox.Name = xPath + "/@" + fields[i];

                    float actorValue = Convert.ToSingle(actorValues.Attributes[fields[i]].Value);
                    textBox.Text = String.Format("{0:0.###}", actorValue);
                    textBox.Location = location;
                    textBox.TextAlign = HorizontalAlignment.Right;
                    textBox.Leave += new EventHandler(NumElementChanged);

                    textBox.Width = boxWidth;

                    m_panel.Controls.Add(textBox);
                }
            }
        } 
    }
}