#include "BombastEditor.h"

BombastEditorApp g_BombastEditorApp;

std::string ROOT_ENGINE_PATH = "../../../../BombastEngine/Engine/";
std::string ROOT_GAME_PATH = "../../../";

CoreGameLogic* BombastEditorApp::VCreateGameAndView()
{
	m_pGame = BE_NEW BombastEditorLogic();
	m_pGame->Initialize();

	//IGameView* gameView = BE_NEW BombastEditorHumanView(g_pApp->GetGraphicsManager()->GetRenderer());
	//m_pGame->VAddView(gameView);

	return m_pGame;
}

BombastEditorLogic::BombastEditorLogic()
{
	m_pGamePhysics = CreateGamePhysics(EPhysicsEngine::BE_PHYSICS_BOMBAST);
	m_pGamePhysics->VSetDebugVisualizationEnabled(true);
}

bool BombastEditorLogic::VLoadGame(const char* levelResource)
{
	while (m_actors.size() > 0)
	{
		ActorId id = m_actors.begin()->first;
		VDestroyActor(id);
	}

	if (!CoreGameLogic::VLoadGame(levelResource))
	{
		return false;
	}

	VChangeState(CGS_Running);
	return true;
}

/*BombastEditorHumanView* BombastEditorLogic::GetHumanView()
{
	//BE_ASSERT(m_gameViews.size() == 1);
	//IGameView* pGameView = *m_gameViews.begin();
	//BombastEditorHumanView* editorHumanView = static_cast<BombastEditorHumanView*>(pGameView);
	return nullptr;
}*/