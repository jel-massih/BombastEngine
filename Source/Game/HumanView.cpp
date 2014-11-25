#include "HumanView.h"

#include "..\UI\UserInterface.h"

HumanView::HumanView(IRenderer* renderer)
{
	m_mouseRadius = 1;
	m_viewId = be_InvalidGameViewId;

	RegisterAllDelegates();
	m_coreGameState = CGS_Initializing;

	if (renderer)
	{
		m_pScene->reset(BE_NEW ScreenElementScene(renderer));
	}
}

HumanView::~HumanView()
{

}

bool HumanView::LoadGame(rapidxml::xml_node<>* pLevelData)
{

}

HRESULT HumanView::VOnRestore()
{

}

HRESULT HumanView::VOnLostDevice()
{

}

void HumanView::VOnRender(double fTime, float fElapsedTime)
{

}

LRESULT CALLBACK HumanView::VOnMsgProc(AppMsg msg)
{

}

void HumanView::VOnUpdate(const int deltaMs)
{

}

void HumanView::VPushElement(IScreenElement* pElement)
{

}

void HumanView::VRemoveElement(IScreenElement* pElement)
{

}

void HumanView::TogglePause(bool active)
{

}

void HumanView::VSetCameraOffset(const Vec4& offset)
{

}

void HumanView::HandleGameState(CoreGameState newState)
{

}

void HumanView::PlaySoundDelegate(IEventDataPtr pEventData)
{

}

void HumanView::GameStateDelegate(IEventDataPtr pEventData)
{

}

void HumanView::RegisterAllDelegates()
{

}

void HumanView::RemoveAllDelegates()
{

}