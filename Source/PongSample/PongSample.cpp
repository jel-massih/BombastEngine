#include "Msvc\PongSampleStd.h"
#include "../Bombast/CoreGameLogic.h"
#include "../Bombast/BombastApp.h"
#include "../Events/Events.h"

#include "Msvc\PongSample.h"
#include "PongSampleView.h"

PongSampleApp g_PongSampleApp;

INT WINAPI wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	return BombastEngine(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

CoreGameLogic *PongSampleApp::VCreateGameAndView()
{
	g_pApp->m_options.m_level = "World\\World.xml";
	
	m_pGame = BE_NEW PongSampleLogic();
	m_pGame->Initialize();

	return m_pGame;
}

PongSampleLogic::PongSampleLogic()
{
	RegisterAllDelegates();
}

PongSampleLogic::~PongSampleLogic()
{
	RemoveAllDelegates();
}

void PongSampleLogic::VChangeState(CoreGameState newState)
{
	CoreGameLogic::VChangeState(newState);

	switch (newState)
	{
		case CGS_WaitingForPlayers:
		{
			BE_ASSERT(m_expectedPlayers == 1);

			for (int i = 0; i < m_expectedPlayers; i++)
			{
				IGameView* playerView = BE_NEW PongSampleHumanView(g_pApp->GetGraphicsManager()->GetRenderer());
				VAddView(playerView);
			}
			
			break;
		}

		case CGS_SpawningPlayersActors:
		{
			for (auto it = m_gameViews.begin(); it != m_gameViews.end(); it++)
			{

			}
			Actor* pActor = VCreateActor("Actors\\paddle.xml", NULL);

			break;
		}

		case CGS_LoadingGameEnvironment:
		{
			VChangeState(CGS_SpawningPlayersActors);
		}
	}
}

bool PongSampleLogic::VLoadGameDelegate(rapidxml::xml_node<>* pLevelData)
{
	return true;
}

void PongSampleLogic::RegisterAllDelegates()
{
	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::RequestStartGameDelegate), EvtData_Request_Start_Game::sk_EventType);
}

void PongSampleLogic::RemoveAllDelegates()
{
	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::RequestStartGameDelegate), EvtData_Request_Start_Game::sk_EventType);
}

void PongSampleLogic::VMoveActor(const ActorId id, Mat4x4 const &mat)
{
	//CoreGameLogic::VMoveActor(id, mat);
}

void PongSampleLogic::RequestStartGameDelegate(IEventDataPtr pEventPtr)
{
	VChangeState(CGS_WaitingForPlayers);
}