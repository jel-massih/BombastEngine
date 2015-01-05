#pragma once

#include "../Game/HumanView.h"
#include "PongSampleController.h"

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
	SceneNode* m_pControlledActor;

private:
	void RegisterAllDelegates();
	void RemoveAllDelegates();
};