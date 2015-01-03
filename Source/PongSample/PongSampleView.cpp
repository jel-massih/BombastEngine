#include "PongSampleView.h"
#include "../Bombast/interfaces.h"
#include "../Graphics3D/Raycast.h"
#include "../Game/ProcessManager.h"
#include "Msvc/PongSample.h"
#include "PongSampleEvents.h"

#include "../Utilities/FastDelegate/FastDelegate.h"
#include "../MultiThreading/RealtimeProcess.h"

PongSampleHumanView::PongSampleHumanView(IRenderer* renderer) :
HumanView(renderer)
{
	m_pPongController = 0;
	m_bShowUI = true;
	RegisterAllDelegates();
}

PongSampleHumanView::~PongSampleHumanView()
{
	RemoveAllDelegates();

	SAFE_DELETE(m_pPongController);
}

LRESULT CALLBACK PongSampleHumanView::VOnMsgProc(AppMsg msg)
{
	if (HumanView::VOnMsgProc(msg))
	{
		return 1;
	}

	if (msg.m_uMsg == WM_KEYDOWN)
	{
		if (msg.m_wParam)
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
			Resource resource("scripts\\test.lua");
			ResourceHandle* pResourceHandle = g_pApp->m_pResourceCache->GetHandle(&resource);
		}
		else if (msg.m_wParam == VK_F8)
		{
			PongSampleLogic* psl = static_cast<PongSampleLogic*>(g_pApp->m_pGame);
			psl->ToggleRenderDiagnostice();
		}
		else if (msg.m_wParam == VK_F9)
		{
			m_pKeyboardHandler = m_pPongController;
			m_pMouseHandler = m_pPongController;
			m_pCamera->SetTarget(m_pPaddle);
			m_pPaddle->SetAlpha(0.8f);
			ReleaseCapture();
			return 1;
		}
		else if (msg.m_wParam == VK_F10)
		{
			//Free Cam
		}
		else if (msg.m_wParam == VK_F12 || msg.m_wParam == VK_ESCAPE)
		{
			if (MessageBox(g_pApp->GetHwnd(), L"Are you sure you want to quit?", L"Quit", MB_YESNO) == IDYES)
			{
				//Quit Game
			}

			return 1;
		}
	}

	return 0;
}

void PongSampleHumanView::VOnUpdate(unsigned long deltaMs)
{
	HumanView::VOnUpdate(deltaMs);

	if (m_pPongController)
	{
		m_pPongController->OnUpdate(deltaMs);
	}

	//Tick Event ?
}

void PongSampleHumanView::VOnAttach(GameViewId vid, ActorId aid)
{
	HumanView::VOnAttach(vid, aid);
}

bool PongSampleHumanView::VLoadGameDelegate(rapidxml::xml_node<>* pLevelData)
{
	if(!HumanView::VLoadGameDelegate(pLevelData))
	{
		return false;
	}

	m_pScene->VOnRestore();
	return true;
}

void PongSampleHumanView::VSetControlledActor(ActorId actorId)
{
	m_pPaddle = static_cast<SceneNode*>(m_pScene->FindActor(actorId));
	if (!m_pPaddle)
	{
		BE_ERROR("Invalid Paddle");
		return;
	}

	HumanView::VSetControlledActor(actorId);

	SAFE_DELETE(m_pPongController);

	m_pPongController = BE_NEW PongSampleController(m_pPaddle);
	m_pKeyboardHandler = m_pPongController;
	m_pMouseHandler = m_pPongController;
}

void PongSampleHumanView::SetControlledActorDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_Set_Controlled_Actor> pCastEventData = std::static_pointer_cast<EvtData_Set_Controlled_Actor>(pEventData);
	VSetControlledActor(pCastEventData->GetActorId());
}

void PongSampleHumanView::RegisterAllDelegates()
{
	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &PongSampleHumanView::SetControlledActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}

void PongSampleHumanView::RemoveAllDelegates()
{
	IEventManager* pEventManager = IEventManager::Get();

	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &PongSampleHumanView::SetControlledActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}