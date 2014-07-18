#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "RenderComponentInterface.h"
#include "../Utilities/rapidxml.hpp"

class BaseRenderComponent : public RenderComponentInterface
{
public:
	virtual bool VInitialize(rapidxml::xml_node<>* pData) override;
	virtual void VPostInit() override;
	virtual void VOnChanged() override;
	virtual rapidxml::xml_node<>* VGenerateXml() override;
	const Color GetColor() const { return m_color; }

protected:
	virtual bool VDelegateInit(rapidxml::xml_node<>* pData) { return true; }
	virtual SceneNode* VCreateSceneNode() = 0;
	Color LoadColor(rapidxml::xml_node<>* pData);

private:
	virtual SceneNode* VGetSceneNode() override;

protected:
	Color m_color;
	SceneNode* m_pSceneNode;
};

#endif