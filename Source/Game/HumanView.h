#pragma once

#include "../Bombast/BombastApp.h"

class HumanView : IGameView
{
	friend class BombastApp;

public:
	HumanView(IRenderer* renderer);
	virtual ~HumanView();

	bool LoadGame(rapidxml::xml_node<>* pLevelData);

	virtual HRESULT VOnRestore();
	virtual HRESULT VOnLostDevice();
	virtual void VOnRender(double fTime, float fElapsedTime);
	virtual GameViewType VGetType() { return GameView_Human; }
	virtual GameViewId VGetId() const { return m_viewId; }

	virtual void VOnAttach(GameViewId vid, ActorId aid)
	{
		m_viewId = vid;
		m_actorId = aid;
	}
	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	virtual void VOnUpdate(const int deltaMs);

	virtual void VPushElement(IScreenElement* pElement);
	virtual void VRemoveElement(IScreenElement* pElement);

	void TogglePause(bool active);

	virtual void VSetCameraOffset(const Vec4& offset);

	void HandleGameState(CoreGameState newState);

	virtual void VSetControlledActor(ActorId actorId) { m_actorId = actorId; }

	void PlaySoundDelegate(IEventDataPtr pEventData);
	void GameStateDelegate(IEventDataPtr pEventData);

protected:
	virtual bool VLoadGameDelegate(rapidxml::xml_node<>* pLevelData) {}

private:
	void RegisterAllDelegates();
	void RemoveAllDelegates();

public:
	ScreenElementList m_screenElements;

	IMouseHandler* m_pMouseHandler;
	int m_mouseRadius;
	IKeyboardHandler* m_pKeyboardHandler;

	ScreenElementScene* m_pScene;
	CameraNode* m_pCamera;

protected:
	GameViewId m_viewId;
	ActorId m_actorId;

	DWORD m_currTick;
	DWORD m_lastDraw;
	bool m_runFullSpeed;

	CoreGameState m_coreGameState;
};