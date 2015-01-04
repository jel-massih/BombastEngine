#pragma once

#include "RenderComponentInterface.h"
#include "../Utilities/rapidxml.hpp"
#include "../Graphics3D/BitmapNode.h"

class BaseRenderComponent : public RenderComponentInterface
{
public:
	BaseRenderComponent();
	~BaseRenderComponent();

	virtual bool VInitialize(rapidxml::xml_node<>* pData) override;
	virtual void VPostInit() override;
	virtual void VOnChanged() override;
	virtual rapidxml::xml_node<>* VGenerateXml() override;
	const Color GetColor() const { return m_color; }

protected:
	virtual bool VDelegateInitialize(rapidxml::xml_node<>* pData) { return true; }
	virtual SceneNode* VCreateSceneNode() = 0;
	Color LoadColor(rapidxml::xml_node<>* pData);

	virtual rapidxml::xml_node<>* VCreateBaseElement(rapidxml::xml_document<>* outDoc) {
		return outDoc->allocate_node(rapidxml::node_element, VGetName());
	}
	virtual void VCreateInheritedXmlElements(rapidxml::xml_node<>* pBaseElement) = 0;

private:
	virtual SceneNode* VGetSceneNode() override;

protected:
	Color m_color;
	SceneNode* m_pSceneNode;
};

class InvisibleRenderComponent : public BaseRenderComponent
{
public:
	static const char* g_Name;
	virtual const char* VGetName() const override{ return InvisibleRenderComponent::g_Name; }

protected:
	virtual SceneNode* VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElements(rapidxml::xml_node<>* pBaseElement) override;
};

class BitmapRenderComponent : public BaseRenderComponent
{
	char* m_textureResource;
	DirectX::XMFLOAT2 m_relativeSize;

public:
	static const char* g_Name;
	virtual const char* VGetName() const override{ return BitmapRenderComponent::g_Name; }

protected:
	virtual bool VDelegateInitialize(rapidxml::xml_node<>* pData) override;
	virtual SceneNode* VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElements(rapidxml::xml_node<>* pBaseElement) override;
};

class BlockRenderComponent : public BaseRenderComponent
{
	char* m_textureResource;
	Vec3 m_size;

public:
	static const char* g_Name;
	virtual const char* VGetName() const override{ return BlockRenderComponent::g_Name; }

protected:
	virtual bool VDelegateInitialize(rapidxml::xml_node<>* pData) override;
	virtual SceneNode* VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElements(rapidxml::xml_node<>* pBaseElement) override;
};