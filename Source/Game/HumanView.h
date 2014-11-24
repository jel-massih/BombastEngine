#pragma once

#include "../Bombast/BombastApp.h"

class HumanView : IGameView
{
	friend class BombastApp;

protected:
	GameViewId m_viewId;
	ActorId m_actorId;

	DWORD m_currTick;
	DWORD m_lastDraw;
	bool m_runFullSpeed;

	CoreGameState m_coreGameState;

public:
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

protected:
	virtual bool VLoadGameDelegate(rapidxml::xml_node<>* pLevelData) {}
};