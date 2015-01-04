#include "Msvc\PongSampleStd.h"
#include "../Bombast/CoreGameLogic.h"
#include "../Bombast/BombastApp.h"
#include "../Events/Events.h"
#include "PongSampleEvents.h"
#include "../Actor/TransformComponent.h"
#include "../Physics/Physics.h"

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
	m_pGamePhysics = CreateGamePhysics();
	RegisterAllDelegates();
}

PongSampleLogic::~PongSampleLogic()
{
	RemoveAllDelegates();
	SAFE_DELETE(m_pGamePhysics);
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
					Actor* pActor = VCreateActor("Actors\\freeCamera.xml", NULL);
					if (pActor)
					{
						std::shared_ptr<EvtData_New_Actor> pNewActorEvent(BE_NEW EvtData_New_Actor(pActor->GetId(), pView->VGetId()));
						IEventManager::Get()->VTriggerEvent(pNewActorEvent);
					
						std::shared_ptr<EvtData_Set_Controlled_Actor> pSetControlledActorEvent(BE_NEW EvtData_Set_Controlled_Actor(pActor->GetId()));
						IEventManager::Get()->VTriggerEvent(pSetControlledActorEvent);
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

bool PongSampleLogic::VLoadGameDelegate(rapidxml::xml_node<>* pLevelData)
{
	return true;
}

void PongSampleLogic::RegisterAllDelegates()
{
	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::RequestStartGameDelegate), EvtData_Request_Start_Game::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::StartMoveUpDelegate), EvtData_StartUp::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::EndMoveUpDelegate), EvtData_EndUp::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::StartMoveRightDelegate), EvtData_StartRight::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::EndMoveRightDelegate), EvtData_EndRight::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::StartMoveForwardDelegate), EvtData_StartForward::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::EndMoveForwardDelegate), EvtData_EndForward::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::StartTurnUpDelegate), EvtData_StartTurnUp::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::EndTurnUpDelegate), EvtData_EndTurnUp::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::StartTurnRightDelegate), EvtData_StartTurnRight::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::EndTurnRightDelegate), EvtData_EndTurnRight::sk_EventType);
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

void PongSampleLogic::RequestStartGameDelegate(IEventDataPtr pEventData)
{
	VChangeState(CGS_WaitingForPlayers);
}

void PongSampleLogic::StartMoveUpDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartUp> pStartUpData = std::static_pointer_cast<EvtData_StartUp>(pEventData);
	Actor* pTarget = VGetActor(pStartUpData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->MoveUp(pStartUpData->GetAcceleration());
	}
}

void PongSampleLogic::EndMoveUpDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndUp> pEndUpData = std::static_pointer_cast<EvtData_EndUp>(pEventData);

}

void PongSampleLogic::StartMoveRightDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartRight> pStartRightData = std::static_pointer_cast<EvtData_StartRight>(pEventData);
	Actor* pTarget = VGetActor(pStartRightData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->MoveLeft(pStartRightData->GetAcceleration());
	}
}

void PongSampleLogic::EndMoveRightDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndRight> pEndRightData = std::static_pointer_cast<EvtData_EndRight>(pEventData);
}

void PongSampleLogic::StartMoveForwardDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartForward> pStartForwardData = std::static_pointer_cast<EvtData_StartForward>(pEventData);
	Actor* pTarget = VGetActor(pStartForwardData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->MoveForward(pStartForwardData->GetAcceleration());
	}
}

void PongSampleLogic::EndMoveForwardDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndForward> pEndForwardData = std::static_pointer_cast<EvtData_EndForward>(pEventData);
}

void PongSampleLogic::StartTurnUpDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartTurnUp> pStartUpData = std::static_pointer_cast<EvtData_StartTurnUp>(pEventData);
	Actor* pTarget = VGetActor(pStartUpData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->RotateUp(pStartUpData->GetAcceleration());
	}
}

void PongSampleLogic::EndTurnUpDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndTurnUp> pEndUpData = std::static_pointer_cast<EvtData_EndTurnUp>(pEventData);
}

void PongSampleLogic::StartTurnRightDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartTurnRight> pStartRightData = std::static_pointer_cast<EvtData_StartTurnRight>(pEventData);
	Actor* pTarget = VGetActor(pStartRightData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->RotateRight(pStartRightData->GetAcceleration());
	}
}

void PongSampleLogic::EndTurnRightDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndTurnRight> pEndRightData = std::static_pointer_cast<EvtData_EndTurnRight>(pEventData);
}