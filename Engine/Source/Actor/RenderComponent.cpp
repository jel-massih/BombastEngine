#include "RenderComponent.h"
#include "../Bombast/BombastApp.h"
#include "TransformComponent.h"
#include "..\Events\Events.h"
#include "../Graphics3D/RenderNodes.h"
#include "../Graphics3D/Lighting.h"

const char* InvisibleRenderComponent::g_Name = "InvisibleRenderComponent";
const char* BitmapRenderComponent::g_Name = "BitmapRenderComponent";
const char* BlockRenderComponent::g_Name = "BlockRenderComponent";
const char* MeshRenderComponent::g_Name = "MeshRenderComponent";
const char* LightRenderComponent::g_Name = "LightRenderComponent";

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
	return NULL;
}

SceneNode* BaseRenderComponent::VGetSceneNode()
{
	if (!m_pSceneNode)
	{
		m_pSceneNode = VCreateSceneNode();
	}

	return m_pSceneNode;
}

Vec4 BaseRenderComponent::LoadColor(rapidxml::xml_node<>* pData)
{
	Vec4 color;

	double r = 1.0;
	double g = 1.0;
	double b = 1.0;
	double a = 1.0;

	r = atof(pData->first_attribute("r")->value());
	g = atof(pData->first_attribute("g")->value());
	b = atof(pData->first_attribute("b")->value());
	a = atof(pData->first_attribute("a")->value());

	color.x = (float)r;
	color.y = (float)g;
	color.z = (float)b;
	color.w = (float)a;

	return color;
}

SceneNode* InvisibleRenderComponent::VCreateSceneNode()
{
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name);

	if (pTransformComponent)
	{
		SceneNode* blankNode = BE_NEW SceneNode(m_pOwner->GetId(), (BaseRenderComponent*) this, RenderPass_Actor, &pTransformComponent->GetTransform());
		return blankNode;
	}

	return NULL;
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
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name);

	if (pTransformComponent)
	{
		SceneNode* bitmap = BE_NEW D3DBitmapNode11(m_pOwner->GetId(), (BaseRenderComponent*)this, m_textureResource, m_relativeSize, RenderPass_GUI, &pTransformComponent->GetTransform());
		return bitmap;
	}

	return NULL;
}

bool BlockRenderComponent::VDelegateInitialize(rapidxml::xml_node<>* pData)
{
	bool result = false;

	rapidxml::xml_node<>* pMaterial = pData->first_node("Material");
	if (pMaterial)
	{
		rapidxml::xml_attribute<char>* test = pMaterial->first_attribute("path");
		if (pMaterial->first_attribute("path") != NULL)
		{
			m_materialResource = pMaterial->first_attribute("path")->value();
		}
		else
		{
			BE_ERROR("Material path Exception: Make sure path attribute is set for Material Element");
			return false;
		}
	}

	rapidxml::xml_node<>* pSize = pData->first_node("Size");
	if (pSize)
	{
		m_size.x = (float)atof(pSize->first_attribute("x")->value());
		m_size.y = (float)atof(pSize->first_attribute("y")->value());
		m_size.z = (float)atof(pSize->first_attribute("z")->value());
	}

	return true;
}

SceneNode* BlockRenderComponent::VCreateSceneNode()
{
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name);

	if (pTransformComponent)
	{
		SceneNode* blockNode = BE_NEW D3D11PrimitiveNode(m_pOwner->GetId(), (BaseRenderComponent*)this, m_materialResource, RenderPass_Static, D3D11PrimitiveNode::PrimitiveType::PT_Box, m_size, &pTransformComponent->GetTransform());
		return blockNode;
	}

	return NULL;
}

bool MeshRenderComponent::VDelegateInitialize(rapidxml::xml_node<>* pData)
{
	bool result = false;

	rapidxml::xml_node<>* pMesh = pData->first_node("Mesh");
	if (pMesh)
	{
		if (pMesh->first_attribute("path") != NULL)
		{
			m_meshResource = pMesh->first_attribute("path")->value();
		}
		else
		{
			BE_ERROR("Mesh path Exception: Make sure path attribute is set for Mesh Element");
			return false;
		}
	}

	rapidxml::xml_node<>* pMaterials = pData->first_node("Materials");
	if (pMaterials)
	{
		for (rapidxml::xml_node<> *child = pMaterials->first_node(); child; child = child->next_sibling())
		{
			if (child->first_attribute("path") != NULL)
			{
				m_materialResources.push_back(child->first_attribute("path")->value());
			}
			else
			{
				BE_ERROR("Material path Exception: Make sure path attribute is set for Material Element");
			}
		}
	}

	return true;
}

SceneNode* MeshRenderComponent::VCreateSceneNode()
{
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name);

	if (pTransformComponent)
	{
		SceneNode* mesh = BE_NEW D3DMeshNode11(m_pOwner->GetId(), (BaseRenderComponent*)this, m_meshResource, m_materialResources, RenderPass_Actor, &pTransformComponent->GetTransform());
		return mesh;
	}

	return NULL;
}

LightRenderComponent::LightRenderComponent()
{
	m_properties.m_range = 10;
	m_properties.m_falloff = 0.1f;
	m_properties.m_lightType = LightProperties::GetLightTypeFromString("directional");
	m_properties.m_diffuseColor = g_White;
	m_properties.m_specularColor = g_White;
	m_properties.m_specularPower = 32;
}

bool LightRenderComponent::VDelegateInitialize(rapidxml::xml_node<>* pData)
{
	rapidxml::xml_node<>* pShape = pData->first_node("Shape");
	if (pShape)
	{
		if (pShape->first_attribute("falloff"))
		{
			m_properties.m_falloff = (float)atof(pShape->first_attribute("falloff")->value());
		}
		if (pShape->first_attribute("range"))
		{
			m_properties.m_range = (float)atof(pShape->first_attribute("range")->value());
		}
	}

	rapidxml::xml_node<>* pType = pData->first_node("Type");
	if (pType)
	{
		m_properties.m_lightType = LightProperties::GetLightTypeFromString(pType->value());
	}

	rapidxml::xml_node<>* pProperties = pData->first_node("Properties");
	if (pProperties)
	{
		rapidxml::xml_node<>* pDiffuseElement = pProperties->first_node("Diffuse");
		if (pDiffuseElement)
		{
			float r, g, b, a;
			r = g = b = a = 1;
			r = std::stof(pDiffuseElement->first_attribute("r")->value());
			g = std::stof(pDiffuseElement->first_attribute("g")->value());
			b = std::stof(pDiffuseElement->first_attribute("b")->value());
			a = std::stof(pDiffuseElement->first_attribute("a")->value());
			m_properties.m_diffuseColor = Vec4(r, g, b, a);
		}

		rapidxml::xml_node<>* pSpecularElement = pProperties->first_node("Specular");
		if (pSpecularElement)
		{
			float r, g, b, a, power;
			r = g = b = a = 1;
			power = 32;
			r = std::stof(pSpecularElement->first_attribute("r")->value());
			g = std::stof(pSpecularElement->first_attribute("g")->value());
			b = std::stof(pSpecularElement->first_attribute("b")->value());
			a = std::stof(pSpecularElement->first_attribute("a")->value());
			power = std::stof(pSpecularElement->first_attribute("power")->value());
			m_properties.m_specularColor = Vec4(r, g, b, a);
			m_properties.m_specularPower = power;
		}
	}

	return true;
}

SceneNode* LightRenderComponent::VCreateSceneNode()
{
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name);

	if (pTransformComponent && m_properties.m_lightType != BLT_INVALID)
	{
		SceneNode* light = BE_NEW D3DLightNode11(m_pOwner->GetId(), (BaseRenderComponent*)this, m_properties, &pTransformComponent->GetTransform());
		return light;
	}

	return NULL;
}