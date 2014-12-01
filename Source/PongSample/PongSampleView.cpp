#include "PongSampleView.h"
#include "../Bombast/interfaces.h"
#include "../Graphics3D/Raycast.h"
#include "../Game/ProcessManager.h"
#include "Msvc/PongSample.h"

PongSampleHumanView::PongSampleHumanView(IRenderer* renderer) :
HumanView(renderer)
{
	m_bShowUI = true;
	RegisterAllDelegates();
}

PongSampleHumanView::~PongSampleHumanView()
{
	RemoveAllDelegates();
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
	}
}