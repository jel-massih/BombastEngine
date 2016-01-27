#include "GameSampleView.h"
#include "Bombast/interfaces.h"
#include "Graphics3D/Raycast.h"
#include "Graphics3D/RenderNodes.h"
#include "Graphics3D/MovementController.h"
#include "Game/ProcessManager.h"
#include "Resources/ResourceCache.h"
#include "Msvc/GameSample.h"
#include "GameSampleEvents.h"
#include "GameSampleController.h"

#include "Utilities/FastDelegate/FastDelegate.h"
#include "MultiThreading/RealtimeProcess.h"


GameSampleHumanView::GameSampleHumanView(IRenderer* renderer) :
HumanView(renderer)
{
	m_pPlayerController = 0;
	m_pFreeCameraController = 0;
	m_pGrid = nullptr;
	m_bShowUI = true;
	m_bShowGrid = false;
	RegisterAllDelegates();
}

GameSampleHumanView::~GameSampleHumanView()
{
	RemoveAllDelegates();

	SAFE_DELETE(m_pFreeCameraController);
	SAFE_DELETE(m_pPlayerController);
}

LRESULT CALLBACK GameSampleHumanView::VOnMsgProc(AppMsg msg)
{
	if (HumanView::VOnMsgProc(msg))
	{
		return 1;
	}

	if (msg.m_uMsg == WM_KEYDOWN)
	{
		if (msg.m_wParam == VK_F1)
		{
			m_bShowUI = !m_bShowUI;
			return 1;
		}
		else if (msg.m_wParam == VK_F2)
		{
			POINT ptCursor;
			GetCursorPos(&ptCursor);
			ScreenToClient(g_pApp->GetHwnd(), &ptCursor);

			RayCast rayCast(ptCursor);
			m_pScene->Pick(&rayCast);
			rayCast.Sort();

			if (rayCast.m_numIntersections)
			{
				int a = 0;
			}
		}
		else if (msg.m_wParam == VK_F3)
		{
			extern void testThreading(ProcessManager* procManager, bool runProtected);
			testThreading(m_pProcessManager, true);
			return 1;

			Sleep(5000);
			testThreading(m_pProcessManager, false);
			Sleep(5000);
		}
		else if (msg.m_wParam == VK_F4)
		{
			m_bShowGrid = !m_bShowGrid;
			m_pGrid->SetVisible(m_bShowGrid);
		}
		else if (msg.m_wParam == VK_F5)
		{
			g_pApp->GetGraphicsManager()->GetRenderer()->VToggleFillMode();
		}
		else if (msg.m_wParam == VK_F8)
		{
			GameSampleLogic* psl = static_cast<GameSampleLogic*>(g_pApp->m_pGame);
			psl->ToggleRenderDiagnostice();
		}
		else if (msg.m_wParam == VK_F9)
		{
			m_pKeyboardHandler = m_pPlayerController;
			m_pMouseHandler = m_pPlayerController;
			m_pCamera->SetViewTarget(m_pControlledActor);
			m_pCamera->SetFollowTarget(m_pControlledActor);
			ReleaseCapture();
			return 1;
		}
		else if (msg.m_wParam == VK_F11)
		{
			m_pKeyboardHandler = m_pFreeCameraController;
			m_pMouseHandler = m_pFreeCameraController;
			m_pCamera->ClearFollowTarget();
			m_pCamera->ClearViewTarget();
			SetCapture(g_pApp->GetHwnd());
			return 1;
		}
		else if (msg.m_wParam == VK_F12 || msg.m_wParam == VK_ESCAPE)
		{
			if (MessageBox(g_pApp->GetHwnd(), L"Are you sure you want to quit?", L"Quit", MB_YESNO) == IDYES)
			{
				g_pApp->AbortGame();
			}

			return 1;
		}
	}

	return 0;
}

void GameSampleHumanView::VOnUpdate(const float deltaMs)
{
	HumanView::VOnUpdate(deltaMs);

	if (m_pFreeCameraController)
	{
		m_pFreeCameraController->OnUpdate(deltaMs);
	}

	if (m_pPlayerController)
	{
		m_pPlayerController->OnUpdate(deltaMs);
	}

	//Tick Event ?
}

void GameSampleHumanView::VOnAttach(GameViewId vid, ActorId aid)
{
	HumanView::VOnAttach(vid, aid);
}

bool GameSampleHumanView::VLoadGameDelegate(rapidxml::xml_node<>* pLevelData)
{
	if(!HumanView::VLoadGameDelegate(pLevelData))
	{
		return false;
	}
	m_pCamera->SetPosition(Vec3(0, 2, 0));

	m_pFreeCameraController = BE_NEW MovementController(m_pCamera, XMConvertToRadians(0), XMConvertToRadians(180), true);

	m_pKeyboardHandler = m_pFreeCameraController;
	m_pMouseHandler = m_pFreeCameraController;
	m_pCamera->ClearFollowTarget();
	m_pCamera->ClearViewTarget();
	SetCapture(g_pApp->GetHwnd());

	m_pGrid = BE_NEW D3D11GridNode(INVALID_ACTOR_ID, nullptr, &Mat4x4::g_Identity);
	m_pScene->AddChild(INVALID_ACTOR_ID, m_pGrid);
	m_pGrid->SetVisible(m_bShowGrid);

	m_pScene->VOnRestore();

	return true;
}

void GameSampleHumanView::VSetControlledActor(ActorId actorId)
{
	m_pControlledActor = static_cast<SceneNode*>(m_pScene->FindActor(actorId));
	if (!m_pControlledActor)
	{
		BE_ERROR("Invalid Desired Controlled Actor");
		return;
	}

	HumanView::VSetControlledActor(actorId);

	SAFE_DELETE(m_pPlayerController);

	m_pPlayerController = BE_NEW GameSampleController(m_pControlledActor);
	m_pKeyboardHandler = m_pPlayerController;
	m_pMouseHandler = m_pPlayerController;

	m_pCamera->SetViewTarget(m_pControlledActor);
	m_pCamera->SetFollowTarget(m_pControlledActor);
	m_pCamera->SetCameraOffset(Vec4(0, 50, 8, 0));
}

void GameSampleHumanView::SetControlledActorDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_Set_Controlled_Actor> pCastEventData = std::static_pointer_cast<EvtData_Set_Controlled_Actor>(pEventData);
	VSetControlledActor(pCastEventData->GetActorId());
}

void GameSampleHumanView::RegisterAllDelegates()
{
	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleHumanView::SetControlledActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &GameSampleHumanView::NewPlayerSpawnedDelegate), EvtData_SpawnPlayer::sk_EventType);
}

void GameSampleHumanView::RemoveAllDelegates()
{
	IEventManager* pEventManager = IEventManager::Get();

	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &GameSampleHumanView::SetControlledActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}

void GameSampleHumanView::NewPlayerSpawnedDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_SpawnPlayer> pCastEventData = std::static_pointer_cast<EvtData_SpawnPlayer>(pEventData);
	if (pCastEventData->GetViewId() == m_viewId) {
		VSetControlledActor(pCastEventData->GetActorId());
	}
}