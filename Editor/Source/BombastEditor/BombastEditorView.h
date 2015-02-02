#pragma once

#include "Game/HumanView.h"

class MovementController;
class D3D11GridNode;

class BombastEditorHumanView : public HumanView
{
public:
	BombastEditorHumanView(IRenderer* renderer);
	virtual ~BombastEditorHumanView();

	virtual void VOnUpdate(const float deltaMs);
	virtual void VOnAttach(GameViewId vid, ActorId aid);

	virtual bool VLoadGameDelegate(rapidxml::xml_node<>* pLevelData) override;

protected:
	bool m_bShowUI, m_bShowGrid;
	std::wstring m_gameplayText;

	MovementController* m_pFreeCameraController;
	SceneNode* m_pControlledActor;

	D3D11GridNode* m_pGrid;
};