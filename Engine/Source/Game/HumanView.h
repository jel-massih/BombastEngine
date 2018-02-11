#pragma once

#include "../Bombast/BombastApp.h"
#include "../UI/UserInterface.h"
#include "ProcessManager.h"
#include "..\Debugging\DebugManager.h"

class HumanView : public IGameView
{
	friend class BombastApp;

public:
	HumanView(IRenderer* renderer);
	virtual ~HumanView();

	bool LoadGame(rapidxml::xml_node<>* pLevelData);

	virtual HRESULT VOnRestore();
	virtual HRESULT VOnLostDevice();
	virtual void VOnRender(const float deltaMs, double elapsedMs);
	virtual GameViewType VGetType() { return GameView_Human; }
	virtual GameViewId VGetId() const { return m_viewId; }

	virtual void VOnAttach(GameViewId vid, ActorId aid)
	{
		m_viewId = vid;
		m_actorId = aid;
	}
	virtual void VOnUpdate(const float deltaMs);

	virtual void VPushElement(IScreenElement* pElement);
	virtual void VRemoveElement(IScreenElement* pElement);

	void TogglePause(bool active);

	virtual void VSetCameraOffset(const Vec4& offset);

	ProcessManager* GetProcessManager() const { return m_pProcessManager; }
	DebugManager*  GetDebugManager() const { return m_pDebugManager; }

	virtual void VSetControlledActor(ActorId actorId) { m_actorId = actorId; }

	void PlaySoundDelegate(EventDataPtr pEventData);

protected:
	virtual bool VLoadGameDelegate(rapidxml::xml_node<>* pLevelData) { VPushElement(m_pScene); return true; }

private:
	void RegisterAllDelegates();
	void RemoveAllDelegates();

public:
	ScreenElementList m_screenElements;

	IInputHandler* m_pInputHandler;

	ScreenElementScene* m_pScene;
	CameraNode* m_pCamera;

protected:
	GameViewId m_viewId;
	ActorId m_actorId;

	ProcessManager* m_pProcessManager;
	DebugManager* m_pDebugManager;

	DWORD m_currTick;
	DWORD m_lastDraw;
	bool m_runFullSpeed;

	CoreGameState m_coreGameState;
};