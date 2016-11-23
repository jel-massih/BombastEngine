#pragma once

#include "RenderComponentInterface.h"
#include "../Utilities/rapidxml.hpp"
#include "../Graphics3D/Lighting.h"

class BaseRenderComponent : public RenderComponentInterface
{
public:
	BaseRenderComponent();
	~BaseRenderComponent();

	virtual bool VInitialize(rapidxml::xml_node<>* pData) override;
	virtual void VPostInit() override;
	virtual void VOnChanged() override;
	virtual rapidxml::xml_node<>* VGenerateXml(rapidxml::xml_document<> &outDoc) { return nullptr; };
	const Vec4 GetColor() const { return m_color; }

protected:
	virtual bool VDelegateInitialize(rapidxml::xml_node<>* pData) { return true; }
	virtual SceneNode* VCreateSceneNode() = 0;
	Vec4 LoadColor(rapidxml::xml_node<>* pData);

private:
	virtual SceneNode* VGetSceneNode() override;

protected:
	Vec4 m_color;
	SceneNode* m_pSceneNode;
};

class InvisibleRenderComponent : public BaseRenderComponent
{
public:
	static const char* g_Name;
	virtual const char* VGetName() const override{ return InvisibleRenderComponent::g_Name; }

	virtual rapidxml::xml_node<>* VGenerateXml(rapidxml::xml_document<> &outDoc) override;

protected:
	virtual SceneNode* VCreateSceneNode() override;
};

class BitmapRenderComponent : public BaseRenderComponent
{
	char* m_textureResource;
	DirectX::XMFLOAT2 m_relativeSize;

public:
	static const char* g_Name;
	virtual const char* VGetName() const override{ return BitmapRenderComponent::g_Name; }

	virtual rapidxml::xml_node<>* VGenerateXml(rapidxml::xml_document<> &outDoc) override;

protected:
	virtual bool VDelegateInitialize(rapidxml::xml_node<>* pData) override;
	virtual SceneNode* VCreateSceneNode() override;
};

class MeshRenderComponent : public BaseRenderComponent
{
	char* m_meshResource;
	std::vector<std::string> m_materialResources;

public:
	MeshRenderComponent() : m_meshResource("") {}

	static const char* g_Name;
	virtual const char* VGetName() const override { return MeshRenderComponent::g_Name; }

	virtual rapidxml::xml_node<>* VGenerateXml(rapidxml::xml_document<> &outDoc) override;

protected:
	virtual bool VDelegateInitialize(rapidxml::xml_node<>* pData) override;
	virtual SceneNode* VCreateSceneNode() override;
};

class LightRenderComponent : public BaseRenderComponent
{
public:
	LightRenderComponent();

	static const char* g_Name;
	virtual const char* VGetName() const { return g_Name; }

	virtual rapidxml::xml_node<>* VGenerateXml(rapidxml::xml_document<> &outDoc) override;

protected:
	virtual bool VDelegateInitialize(rapidxml::xml_node<>* pBaseElement) override;
	virtual SceneNode* VCreateSceneNode() override;

private:
	LightProperties m_properties;
};