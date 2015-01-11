#include "RenderComponent.h"
#include "../Bombast/BombastApp.h"
#include "TransformComponent.h"
#include "..\Events\Events.h"
#include "../Graphics3D/RenderNodes.h"

const char* InvisibleRenderComponent::g_Name = "InvisibleRenderComponent";
const char* BitmapRenderComponent::g_Name = "BitmapRenderComponent";
const char* BlockRenderComponent::g_Name = "BlockRenderComponent";
const char* MeshRenderComponent::g_Name = "MeshRenderComponent";

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

	//@TODO: Refactor code duplication for textures (Separate Component?)
	rapidxml::xml_node<>* pTexture = pData->first_node("Texture");
	if (pTexture)
	{
		rapidxml::xml_attribute<char>* test = pTexture->first_attribute("path");
		if (pTexture->first_attribute("path") != NULL)
		{
			m_textureResource = pTexture->first_attribute("path")->value();
		}
		else
		{
			BE_ERROR("Texture path Exception: Make sure path attribute is set for Texture Element");
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
		SceneNode* blockNode = BE_NEW D3D11PrimitiveNode(m_pOwner->GetId(), (BaseRenderComponent*)this, m_textureResource, RenderPass_Static, D3D11PrimitiveNode::PrimitiveType::PT_Box, m_size, &pTransformComponent->GetTransform());
		return blockNode;
	}

	return NULL;
}

bool MeshRenderComponent::VDelegateInitialize(rapidxml::xml_node<>* pData)
{
	bool result = false;

	rapidxml::xml_node<>* pTexture = pData->first_node("Texture");
	if (pTexture)
	{
		if (pTexture->first_attribute("path") != NULL)
		{
			m_textureResource = pTexture->first_attribute("path")->value();
		}
		else
		{
			BE_ERROR("Texture path Exception: Make sure path attribute is set for Texture Element");
			return false;
		}
	}

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

	return true;
}

SceneNode* MeshRenderComponent::VCreateSceneNode()
{
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name);

	if (pTransformComponent)
	{
		SceneNode* mesh = BE_NEW D3DMeshNode11(m_pOwner->GetId(), (BaseRenderComponent*)this, m_textureResource, m_meshResource, RenderPass_Actor, &pTransformComponent->GetTransform());
		return mesh;
	}

	return NULL;
}