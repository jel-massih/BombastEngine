#include "Msvc\GameSampleStd.h"
#include "../Bombast/CoreGameLogic.h"
#include "../Bombast/BombastApp.h"
#include "../Events/Events.h"
#include "GameSampleEvents.h"
#include "../Actor/TransformComponent.h"
#include "../Actor/PhysicsComponent.h"
#include "../Physics/Physics.h"

#include "Msvc\GameSample.h"
#include "GameSampleView.h"

GameSampleApp g_GameSampleApp;

INT WINAPI wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	return BombastEngine(hInstance, hPrevInstance, NULL);
}

CoreGameLogic *GameSampleApp::VCreateGameAndView()
{
	m_pGame = BE_NEW GameSampleLogic();
	m_pGame->Initialize();

	return m_pGame;
}

GameSampleLogic::GameSampleLogic()
{
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
					Actor* pActor = VCreateActor("actors\\player_skateboard.xml", NULL);
					if (pActor)
					{
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
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::RequestStartGameDelegate), EvtData_Request_Start_Game::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveUpDelegate), EvtData_StartUp::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveUpDelegate), EvtData_EndUp::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveRightDelegate), EvtData_StartRight::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveRightDelegate), EvtData_EndRight::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveForwardDelegate), EvtData_StartForward::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveForwardDelegate), EvtData_EndForward::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartTurnUpDelegate), EvtData_StartTurnUp::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndTurnUpDelegate), EvtData_EndTurnUp::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartTurnRightDelegate), EvtData_StartTurnRight::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndTurnRightDelegate), EvtData_EndTurnRight::sk_EventType);
}

void GameSampleLogic::RemoveAllDelegates()
{
	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::RequestStartGameDelegate), EvtData_Request_Start_Game::sk_EventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveUpDelegate), EvtData_StartUp::sk_EventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveUpDelegate), EvtData_EndUp::sk_EventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveRightDelegate), EvtData_StartRight::sk_EventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveRightDelegate), EvtData_EndRight::sk_EventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartMoveForwardDelegate), EvtData_StartForward::sk_EventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndMoveForwardDelegate), EvtData_EndForward::sk_EventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartTurnUpDelegate), EvtData_StartTurnUp::sk_EventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndTurnUpDelegate), EvtData_EndTurnUp::sk_EventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::StartTurnRightDelegate), EvtData_StartTurnRight::sk_EventType);
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleLogic::EndTurnRightDelegate), EvtData_EndTurnRight::sk_EventType);
} 

void GameSampleLogic::VMoveActor(const ActorId id, Mat4x4 const &mat)
{
	//CoreGameLogic::VMoveActor(id, mat);
}

void GameSampleLogic::RequestStartGameDelegate(IEventDataPtr pEventData)
{
	VChangeState(CGS_WaitingForPlayers);
}

void GameSampleLogic::StartMoveUpDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartUp> pStartUpData = std::static_pointer_cast<EvtData_StartUp>(pEventData);
	Actor* pTarget = VGetActor(pStartUpData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->MoveUp(pStartUpData->GetAcceleration());
	}
}

void GameSampleLogic::EndMoveUpDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndUp> pEndUpData = std::static_pointer_cast<EvtData_EndUp>(pEventData);

}

void GameSampleLogic::StartMoveRightDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartRight> pStartRightData = std::static_pointer_cast<EvtData_StartRight>(pEventData);
	Actor* pTarget = VGetActor(pStartRightData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->MoveLeft(pStartRightData->GetAcceleration());
	}
}

void GameSampleLogic::EndMoveRightDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndRight> pEndRightData = std::static_pointer_cast<EvtData_EndRight>(pEventData);
}

void GameSampleLogic::StartMoveForwardDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartForward> pStartForwardData = std::static_pointer_cast<EvtData_StartForward>(pEventData);
	Actor* pTarget = VGetActor(pStartForwardData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->MoveForward(pStartForwardData->GetAcceleration());
	}
}

void GameSampleLogic::EndMoveForwardDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndForward> pEndForwardData = std::static_pointer_cast<EvtData_EndForward>(pEventData);
	Actor* pTarget = VGetActor(pEndForwardData->GetActorId());
	if (pTarget)
	{
		PhysicsComponent* pPhysicsComponent = pTarget->GetComponent<PhysicsComponent>(PhysicsComponent::g_Name);
		if (pPhysicsComponent)
		{
			pPhysicsComponent->RemoveAcceleration();
		}
	}
}

void GameSampleLogic::StartTurnUpDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartTurnUp> pStartUpData = std::static_pointer_cast<EvtData_StartTurnUp>(pEventData);
	Actor* pTarget = VGetActor(pStartUpData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->RotateUp(pStartUpData->GetAcceleration());
	}
}

void GameSampleLogic::EndTurnUpDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndTurnUp> pEndUpData = std::static_pointer_cast<EvtData_EndTurnUp>(pEventData);
}

void GameSampleLogic::StartTurnRightDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartTurnRight> pStartRightData = std::static_pointer_cast<EvtData_StartTurnRight>(pEventData);
	Actor* pTarget = VGetActor(pStartRightData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->RotateRight(pStartRightData->GetAcceleration());
	}
}

void GameSampleLogic::EndTurnRightDelegate(IEventDataPtr pEventData)
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