#include "RenderComponent.h"

bool BaseRenderComponent::VInitialize(rapidxml::xml_node<>* pData)
{
	rapidxml::xml_node<>* pColorNode = pData->first_node("Color");
	if (pColorNode)
	{
		m_color = LoadColor(pColorNode);
	}

	return VDelegateInitialize(pData);
}

void BaseRenderComponent::VPostInit()
{
	SceneNode* pSceneNode = VGetSceneNode();
	//@TODO:
	//Added event trigger stuff
}

void BaseRenderComponent::VOnChanged()
{
	//@TODO:
	//Add Event Trigger Stuff
}

rapidxml::xml_node<>* BaseRenderComponent::VGenerateXml()
{
	rapidxml::xml_document<> outDoc;
	rapidxml::xml_node<>* pBaseElement = VCreateBaseElement(&outDoc);

	rapidxml::xml_node<>* pColor = outDoc.allocate_node(rapidxml::node_element, "Color");
	pColor->append_attribute(outDoc.allocate_attribute("r", ToStr(m_color.r).c_str()));
	pColor->append_attribute(outDoc.allocate_attribute("g", ToStr(m_color.g).c_str()));
	pColor->append_attribute(outDoc.allocate_attribute("b", ToStr(m_color.b).c_str()));
	pColor->append_attribute(outDoc.allocate_attribute("a", ToStr(m_color.a).c_str()));
	pBaseElement->append_node(pColor);

	VCreateInheritedXmlElements(pBaseElement);

	return pBaseElement;
}

SceneNode* BaseRenderComponent::VGetSceneNode()
{
	if (!m_pSceneNode)
	{
		m_pSceneNode = VCreateSceneNode();
	}

	return m_pSceneNode;
}

Color BaseRenderComponent::LoadColor(rapidxml::xml_node<>* pData)
{
	Color color;

	double r = 1.0;
	double g = 1.0;
	double b = 1.0;
	double a = 1.0;

	r = atof(pData->first_attribute("r")->value());
	g = atof(pData->first_attribute("g")->value());
	b = atof(pData->first_attribute("b")->value());
	a = atof(pData->first_attribute("a")->value());

	color.r = (float)r;
	color.g = (float)g;
	color.b = (float)b;
	color.a = (float)a;

	return color;
}
