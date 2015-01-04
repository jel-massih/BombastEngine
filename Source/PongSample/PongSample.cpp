#include "Msvc\PongSampleStd.h"
#include "../Bombast/CoreGameLogic.h"
#include "../Bombast/BombastApp.h"
#include "../Events/Events.h"
#include "PongSampleEvents.h"
#include "../Actor/TransformComponent.h"

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
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::StartMoveLeftDelegate), EvtData_StartLeft::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::EndMoveLeftDelegate), EvtData_EndLeft::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::StartMoveForwardDelegate), EvtData_StartForward::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleLogic::EndMoveForwardDelegate), EvtData_EndForward::sk_EventType);
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

void PongSampleLogic::StartMoveLeftDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartLeft> pStartLeftData = std::static_pointer_cast<EvtData_StartLeft>(pEventData);
	Actor* pTarget = VGetActor(pStartLeftData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->MoveLeft(pStartLeftData->GetAcceleration());
	}
}

void PongSampleLogic::EndMoveLeftDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndLeft> pEndLeftData = std::static_pointer_cast<EvtData_EndLeft>(pEventData);
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

void PongSampleLogic::StartTurnLeftDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_StartTurnLeft> pStartLeftData = std::static_pointer_cast<EvtData_StartTurnLeft>(pEventData);
	Actor* pTarget = VGetActor(pStartLeftData->GetActorId());
	if (pTarget)
	{
		TransformComponent* pTransformComponent = pTarget->GetComponent<TransformComponent>(TransformComponent::g_Name);
		pTransformComponent->RotateRight(pStartLeftData->GetAcceleration());
	}
}

void PongSampleLogic::EndTurnLeftDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_EndTurnLeft> pEndLeftData = std::static_pointer_cast<EvtData_EndTurnLeft>(pEventData);
}