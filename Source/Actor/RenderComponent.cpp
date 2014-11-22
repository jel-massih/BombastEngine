#include "RenderComponent.h"
#include "../Bombast/BombastApp.h"
#include "TransformComponent.h"
#include "..\Events\Events.h"

const char* BitmapRenderComponent::g_Name = "BitmapRenderComponent";

BaseRenderComponent::BaseRenderComponent()
{
	m_pSceneNode = 0;
}

BaseRenderComponent::~BaseRenderComponent()
{
}

bool BaseRenderComponent::VInitialize(rapidxml::xml_node<>* pData)
{
	rapidxml::xml_node<>* pColorNode = pData->first_node("Color");
	if (pColorNode)
	{
		m_color = LoadColor(pColorNode);
	}
	else
	{
		m_color = g_White;
	}

	return VDelegateInitialize(pData);
}

void BaseRenderComponent::VPostInit()
{
	SceneNode* pSceneNode = VGetSceneNode();
	std::shared_ptr<EvtData_New_Render_Component> pEvent(BE_NEW EvtData_New_Render_Component(m_pOwner->GetId(), pSceneNode));
	IEventManager::Get()->VTriggerEvent(pEvent);
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

bool BitmapRenderComponent::VDelegateInitialize(rapidxml::xml_node<>* pData)
{
	bool result = false;

	rapidxml::xml_node<>* pTexture = pData->first_node("Texture");
	if (pTexture)
	{
		rapidxml::xml_attribute<char>* test = pTexture->first_attribute("path");
		if (pTexture->first_attribute("path") != NULL) {
			m_textureResource = pTexture->first_attribute("path")->value();
		}
		else
		{
			BE_ERROR("Texture Path Exception: Make sure path attribute is set for Texture Element");
			return false;
		}
	}

	rapidxml::xml_node<>* pRelativeSize = pData->first_node("RelativeSize");
	if (pRelativeSize)
	{
		m_relativeSize.x = (float)atof(pRelativeSize->first_attribute("x")->value());
		m_relativeSize.y = (float)atof(pRelativeSize->first_attribute("y")->value());
	}

	return true;
}

SceneNode* BitmapRenderComponent::VCreateSceneNode()
{
	TransformComponent* pTransformCompnent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name);

	if (pTransformCompnent)
	{
		SceneNode* bitmap = BE_NEW D3DBitmapNode11(m_pOwner->GetId(), (BaseRenderComponent*)this, m_textureResource, m_relativeSize, RenderPass_GUI, &pTransformCompnent->GetTransform());
		return bitmap;
	}

	return NULL;
}

void BitmapRenderComponent::VCreateInheritedXmlElements(rapidxml::xml_node<>* pBaseElement)
{
	BE_ERROR("ERROR: Not Implemented Yet");
}