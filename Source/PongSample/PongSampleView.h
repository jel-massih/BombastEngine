#pragma once

#include "../Game/HumanView.h"

class MovementController;
class PongSampleController;

class PongSampleHumanView : public HumanView
{
public:
	PongSampleHumanView(IRenderer* renderer);
	virtual ~PongSampleHumanView();

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	virtual void VOnUpdate(unsigned long deltaMs);
	virtual void VOnAttach(GameViewId vid, ActorId aid);

	virtual void VSetControlledActor(ActorId actorId);
	virtual bool VLoadGameDelegate(rapidxml::xml_node<>* pLevelData) override;

	void SetControlledActorDelegate(IEventDataPtr pEventData);

protected:
	bool m_bShowUI;
	std::wstring m_gameplayText;

	PongSampleController* m_pPongController;
	MovementController* m_pFreeCameraController;
	SceneNode* m_pControlledActor;

private:
	void RegisterAllDelegates();
	void RemoveAllDelegates();
};