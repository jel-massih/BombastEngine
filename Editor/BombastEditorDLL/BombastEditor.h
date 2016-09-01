#pragma once

#include "BombastEngineStd.h"
#include "Bombast/BombastApp.h"

class BombastEditorHumanView;

class BombastEditorApp : public BombastApp
{
public:
	TCHAR* VGetGameTitle() { return _T("Bombast Editor"); }

protected:
	CoreGameLogic* VCreateGameAndView();
};

class BombastEditorLogic : public CoreGameLogic
{
public:
	BombastEditorLogic();

	virtual bool VLoadGame(const char* levelResource);

	int GetNumActors() { return m_actors.size(); }

	const ActorMap& GetActorMap() { return m_actors; }

	bool IsRunning() { return m_gameState == CGS_Running; }
	
	BombastEditorHumanView* GetHumanView();
};