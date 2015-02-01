#include "HumanView.h"

#include <timeapi.h>

const unsigned int MAX_FPS = 60;
const GameViewId be_InvalidGameViewId = 0xffffffff;

HumanView::HumanView(IRenderer* renderer)
{
	m_pProcessManager = BE_NEW ProcessManager;
	m_pDebugManager = BE_NEW DebugManager;

	m_pKeyboardHandler = nullptr;
	m_pMouseHandler = nullptr;

	m_mouseRadius = 1;
	m_viewId = be_InvalidGameViewId;

	RegisterAllDelegates();
	m_coreGameState = CGS_Initializing;

	m_runFullSpeed = true;

	if (renderer)
	{
		m_pScene = BE_NEW ScreenElementScene(renderer);

		Frustum frustum;
		frustum.Init(BE_PI / 4.0f, 1.0f, 1.0f, 100.0f);
		m_pCamera = BE_NEW CameraNode(&Mat4x4::g_Identity, frustum);
		BE_ASSERT_MESSAGE(m_pScene && m_pCamera, "Out of Memory");

		m_pScene->VAddChild(INVALID_ACTOR_ID, m_pCamera);
		m_pScene->SetCamera(m_pCamera);
	}

	bool result;
	result = m_pDebugManager->Initialize(this);
	if (!result)
	{
		BE_ERROR("Failed to Initialze DebugManager");
	}
}

HumanView::~HumanView()
{
	RemoveAllDelegates();
	while (!m_screenElements.empty())
	{
		m_screenElements.pop_front();
	}

	SAFE_DELETE(m_pDebugManager);

	SAFE_DELETE(m_pProcessManager);
	SAFE_DELETE(m_pScene);
}

bool HumanView::LoadGame(rapidxml::xml_node<>* pLevelData)
{
	return VLoadGameDelegate(pLevelData);
}

HRESULT HumanView::VOnRestore()
{
	HRESULT hr;
	for (ScreenElementList::iterator it = m_screenElements.begin(); it != m_screenElements.end(); it++)
	{
		BE_HRETURN((*it)->VOnRestore(), "Failed to restore Screen Element");
	}

	return S_OK;
}

HRESULT HumanView::VOnLostDevice()
{
	HRESULT hr;

	for (ScreenElementList::iterator it = m_screenElements.begin(); it != m_screenElements.end(); it++)
	{
		BE_HRETURN((*it)->VOnRestore(), "Failed to Lose Screen ELement Device");
	}

	return S_OK;
}

void HumanView::VOnRender(const float deltaMs, double elapsedMs)
{
	m_currTick = timeGetTime();
	if (m_currTick == m_lastDraw)
	{
		return;
	}

	if (m_runFullSpeed || ((m_currTick - m_lastDraw) > 1000 / MAX_FPS))
	{
		m_lastDraw = m_currTick;

		m_screenElements.sort(SortBy_Ptr_Content<IScreenElement>());

		for (ScreenElementList::iterator it = m_screenElements.begin(); it != m_screenElements.end(); it++)
		{
			if ((*it)->VIsVisible())
			{
				(*it)->VOnRender(deltaMs, elapsedMs);
			}
		}
		g_pApp->GetGraphicsManager()->GetRenderer()->VEnableZBuffer(false);
		g_pApp->GetGraphicsManager()->GetRenderer()->VEnableAlphaBlending(true);

		if (m_pDebugManager)
		{
			m_pDebugManager->Render();
		}

		g_pApp->GetGraphicsManager()->GetRenderer()->VEnableAlphaBlending(false);
		g_pApp->GetGraphicsManager()->GetRenderer()->VEnableZBuffer(true);

		g_pApp->GetGraphicsManager()->GetRenderer()->VEndScene();
	}
}

LRESULT CALLBACK HumanView::VOnMsgProc(AppMsg msg)
{
	for (ScreenElementList::reverse_iterator it = m_screenElements.rbegin(); it != m_screenElements.rend(); it++)
	{
		if ((*it)->VIsVisible())
		{
			if ((*it)->VOnMsgProc(msg))
			{
				return 1;
			}
		}
	}

	LRESULT result = 0;
	switch (msg.m_uMsg)
	{
	case WM_KEYDOWN:
		if (m_pKeyboardHandler)
		{
			result = m_pKeyboardHandler->VOnKeyDown((BYTE)msg.m_wParam);
		}
		break;

	case WM_KEYUP:
		if (m_pKeyboardHandler)
		{
			result = m_pKeyboardHandler->VOnKeyUp((BYTE)msg.m_wParam);
		}
		break;

	case WM_MOUSEMOVE:
		if (m_pMouseHandler)
		{
			result = m_pMouseHandler->VOnMouseMove(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1);
		}
		break;

	case WM_LBUTTONDOWN:
		if (m_pMouseHandler)
		{
			SetCapture(msg.m_hWnd);
			result = m_pMouseHandler->VOnMouseDown(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
		}
		break;

	case WM_LBUTTONUP:
		if (m_pMouseHandler)
		{
			SetCapture(NULL);
			result = m_pMouseHandler->VOnMouseUp(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
		}
		break;

	case WM_RBUTTONDOWN:
		if (m_pMouseHandler)
		{
			SetCapture(msg.m_hWnd);
			result = m_pMouseHandler->VOnMouseDown(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
		}
		break;

	case WM_RBUTTONUP:
		if (m_pMouseHandler)
		{
			SetCapture(NULL);
			result = m_pMouseHandler->VOnMouseUp(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
		}
		break;

	default:
		return 0;
	}

	return 0;
}

void HumanView::VOnUpdate(const float deltaMs)
{
	m_pProcessManager->UpdateProcesses(deltaMs);

	m_pDebugManager->Update(deltaMs);

	for (ScreenElementList::iterator it = m_screenElements.begin(); it != m_screenElements.end(); it++)
	{
		(*it)->VOnUpdate(deltaMs);
	}
}

void HumanView::VPushElement(IScreenElement* pElement)
{
	m_screenElements.push_front(pElement);
}

void HumanView::VRemoveElement(IScreenElement* pElement)
{
	m_screenElements.remove(pElement);
}

void HumanView::TogglePause(bool active)
{
	//@TODO: Implement Pausing
}

void HumanView::VSetCameraOffset(const Vec4& offset)
{
	BE_ASSERT(m_pCamera);
	if (m_pCamera)
	{
		m_pCamera->SetCameraOffset(offset);
	}
}

void HumanView::PlaySoundDelegate(IEventDataPtr pEventData)
{
	//@TODO: add Sound playing code
}

void HumanView::RegisterAllDelegates()
{
	IEventManager* pGlobalEventManager = IEventManager::Get();
	//@TODO: Register sound delegate
}

void HumanView::RemoveAllDelegates()
{
	IEventManager* pGlobalEventManager = IEventManager::Get();
	//@TODO: Remove sound delegate
}