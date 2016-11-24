#pragma once

#include "Game/HumanView.h"

class MovementController;

class BombastEditorHumanView : public HumanView
{
public:
	BombastEditorHumanView(IRenderer* renderer);

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	virtual void VOnUpdate(const float deltaMs);
	ScreenElementScene* GetScene() { return m_pScene; }

	virtual void VOnAttach(GameViewId vid, ActorId aid);
	virtual bool VLoadGameDelegate(rapidxml::xml_node<>* pLevelData) override;

protected:
	MovementController* m_pFreeCameraController;
};