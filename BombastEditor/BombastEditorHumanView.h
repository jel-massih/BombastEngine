#pragma once

#include "BombastEngineStd.h"
#include "Game/HumanView.h"
#include "Graphics3D/MovementController.h"

class BombastEditorHumanView : public HumanView
{
public:
	BombastEditorHumanView(IRenderer* renderer);
	virtual void VOnUpdate(unsigned long deltaMs);
	ScreenElementScene* GetScene() { return m_pScene; }

protected:
	MovementController* m_pFreeCameraController;

	virtual bool VLoadGameDelegate(rapidxml::xml_node<>* pLevelData) override;
};