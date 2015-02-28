#pragma once

#include "../Game/HumanView.h"

class MovementController;
class GameSampleController;
class D3D11GridNode;

class GameSampleHumanView : public HumanView
{
public:
	GameSampleHumanView(IRenderer* renderer);
	virtual ~GameSampleHumanView();

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	virtual void VOnUpdate(const float deltaMs);
	virtual void VOnAttach(GameViewId vid, ActorId aid);

	virtual void VSetControlledActor(ActorId actorId);
	virtual bool VLoadGameDelegate(rapidxml::xml_node<>* pLevelData) override;


protected:
	bool m_bShowUI, m_bShowGrid;
	std::wstring m_gameplayText;

	GameSampleController* m_pPlayerController;
	MovementController* m_pFreeCameraController;
	SceneNode* m_pControlledActor;

	D3D11GridNode* m_pGrid;

private:
	void RegisterAllDelegates();
	void RemoveAllDelegates();

	void SetControlledActorDelegate(IEventDataPtr pEventData);
	void NewPlayerSpawnedDelegate(IEventDataPtr pEventData);
};