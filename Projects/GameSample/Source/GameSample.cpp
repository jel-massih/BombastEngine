#include "Msvc\GameSampleStd.h"
#include "Bombast/CoreGameLogic.h"
#include "Bombast/BombastApp.h"
#include "Events/Events.h"
#include "GameSampleEvents.h"
#include "Actor/TransformComponent.h"
#include "Actor/PhysicsComponent.h"
#include "Actor/PhysXCharacterControllerComponent.h"
#include "Physics/Physics.h"

#include "Msvc\GameSample.h"
#include "GameSampleView.h"

GameSampleApp g_GameSampleApp;

INT WINAPI wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	return BombastEngine(hInstance, hPrevInstance, NULL, lpCmdLine);
}

std::string ROOT_ENGINE_PATH = "../../../../../Engine/";
std::string ROOT_GAME_PATH = "../../../";

CoreGameLogic *GameSampleApp::VCreateGameAndView()
{
	m_pGame = BE_NEW GameSampleLogic();
	m_pGame->Initialize();

	return m_pGame;
}

GameSampleLogic::GameSampleLogic()
{
	m_pControllerManager = nullptr;

	m_pGamePhysics = CreateGamePhysics(g_pApp->m_options.m_physicsEngine);
	m_pGamePhysics->VSetDebugVisualizationEnabled(g_pApp->m_options.m_bVisualizeDebugPhysics);
	RegisterAllDelegates();
}

GameSampleLogic::~GameSampleLogic()
{
	RemoveAllDelegates();
}

void GameSampleLogic::VChangeState(CoreGameState newState)
{
	CoreGameLogic::VChangeState(newState);

	switch (newState)
	{
		case CGS_WaitingForPlayers:
		{
			BE_ASSERT(m_expectedPlayers == 1);

			for (int i = 0; i < m_expectedPlayers; i++)
			{
				IGameView* playerView = BE_NEW GameSampleHumanView(g_pApp->GetGraphicsManager()->GetRenderer());
				VAddView(playerView);
				m_humanPlayersAttached++;
			}
			
			break;
		}

		case CGS_SpawningPlayersActors:
		{
			for (auto it = m_gameViews.begin(); it != m_gameViews.end(); it++)
			{
				IGameView* pView = *it;
				if (pView->VGetType() == GameView_Human)
				{
					Actor* pActor = VCreateActor("gamesample.actors.player.xml", NULL);
					if (pActor)
					{
						pActor->GetComponent<TransformComponent>(TransformComponent::g_Name)->SetPosition(Vec3(0, 3, 0));
						std::shared_ptr<EvtData_SpawnPlayer> pSpawnPlayerEvent(BE_NEW EvtData_SpawnPlayer(pActor->GetId(), pView->VGetId()));
						IEventManager::Get()->VTriggerEvent(pSpawnPlayerEvent);
					}
				}
			}
			break;
		}

		case CGS_LoadingGameEnvironment:
		{
			VChangeState(CGS_SpawningPlayersActors);
			break;
		}

		case CGS_Initializing:
		{
			std::shared_ptr<EvtData_Request_Start_Game> pRequestStartGameEvent(BE_NEW EvtData_Request_Start_Game());
			IEventManager::Get()->VTriggerEvent(pRequestStartGameEvent);
			break;
		}
	}
}

bool GameSampleLogic::VLoadGameDelegate(rapidxml::xml_node<>* pLevelData)
{
	return true;
}

void GameSampleLogic::RegisterAllDelegates()
{
	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::RequestStartGameDelegate), EvtData_Request_Start_Game::sEventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveUpDelegate), EvtData_StartUp::sEventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveUpDelegate), EvtData_EndUp::sEventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveRightDelegate), EvtData_StartRight::sEventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveRightDelegate), EvtData_EndRight::sEventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveForwardDelegate), EvtData_StartForward::sEventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveForwardDelegate), EvtData_EndForward::sEventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartTurnUpDelegate), EvtData_StartTurnUp::sEventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndTurnUpDelegate), EvtData_EndTurnUp::sEventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartTurnRightDelegate), EvtData_StartTurnRight::sEventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndTurnRightDelegate), EvtData_EndTurnRight::sEventType);
}

void GameSampleLogic::RemoveAllDelegates()
{
	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::RequestStartGameDelegate), EvtData_Request_Start_Game::sEventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveUpDelegate), EvtData_StartUp::sEventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveUpDelegate), EvtData_EndUp::sEventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveRightDelegate), EvtData_StartRight::sEventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveRightDelegate), EvtData_EndRight::sEventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveForwardDelegate), EvtData_StartForward::sEventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveForwardDelegate), EvtData_EndForward::sEventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartTurnUpDelegate), EvtData_StartTurnUp::sEventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndTurnUpDelegate), EvtData_EndTurnUp::sEventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartTurnRightDelegate), EvtData_StartTurnRight::sEventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndTurnRightDelegate), EvtData_EndTurnRight::sEventType);
} 

void GameSampleLogic::VMoveActor(const ActorId id, Mat4x4 const &mat)
{
	//CoreGameLogic::VMoveActor(id, mat);
}

void GameSampleLogic::RequestStartGameDelegate(EventDataPtr pEventData)
{
	VChangeState(CGS_WaitingForPlayers);
}

void GameSampleLogic::StartMoveUpDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartUp> pStartUpData = std::static_pointer_cast<EvtData_StartUp>(pEventData);
	Actor* pTarget = VGetActor(pStartUpData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->MoveUp(pStartUpData->GetAcceleration());
	}
}

void GameSampleLogic::EndMoveUpDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndUp> pEndUpData = std::static_pointer_cast<EvtData_EndUp>(pEventData);

}

void GameSampleLogic::StartMoveRightDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartRight> pStartRightData = std::static_pointer_cast<EvtData_StartRight>(pEventData);
	Actor* pTarget = VGetActor(pStartRightData->GetActorId());
	if (pTarget)
	{
	}
}

void GameSampleLogic::EndMoveRightDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndRight> pEndRightData = std::static_pointer_cast<EvtData_EndRight>(pEventData);

	Actor* pTarget = VGetActor(pEndRightData->GetActorId());
	if (pTarget)
	{
	}
}

void GameSampleLogic::StartMoveForwardDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartForward> pStartForwardData = std::static_pointer_cast<EvtData_StartForward>(pEventData);
	Actor* pTarget = VGetActor(pStartForwardData->GetActorId());
	if (pTarget)
	{
	}
}

void GameSampleLogic::EndMoveForwardDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndForward> pEndForwardData = std::static_pointer_cast<EvtData_EndForward>(pEventData);
	Actor* pTarget = VGetActor(pEndForwardData->GetActorId());
	if (pTarget)
	{
	}
}

void GameSampleLogic::StartTurnUpDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartTurnUp> pStartUpData = std::static_pointer_cast<EvtData_StartTurnUp>(pEventData);
	Actor* pTarget = VGetActor(pStartUpData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->RotateUp(pStartUpData->GetAcceleration());
	}
}

void GameSampleLogic::EndTurnUpDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndTurnUp> pEndUpData = std::static_pointer_cast<EvtData_EndTurnUp>(pEventData);
}

void GameSampleLogic::StartTurnRightDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartTurnRight> pStartRightData = std::static_pointer_cast<EvtData_StartTurnRight>(pEventData);
	Actor* pTarget = VGetActor(pStartRightData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->RotateRight(pStartRightData->GetAcceleration());
	}
}

void GameSampleLogic::EndTurnRightDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndTurnRight> pEndRightData = std::static_pointer_cast<EvtData_EndTurnRight>(pEventData);
	Actor* pTarget = VGetActor(pEndRightData->GetActorId());
	if (pTarget)
	{
		PhysicsComponent* pPhysicsComponent = pTarget->GetComponent<PhysicsComponent>(PhysicsComponent::g_Name);
		if (pPhysicsComponent)
		{
			pPhysicsComponent->RemoveAngularAcceleration();
		}
	}
}