#include "BombastEditorHumanView.h"
#include "Graphics3D/MovementController.h"

BombastEditorHumanView::BombastEditorHumanView(IRenderer* renderer)
	: HumanView(renderer)
{

}

LRESULT CALLBACK BombastEditorHumanView::VOnMsgProc(AppMsg msg)
{
	return 0;
}

void BombastEditorHumanView::VOnUpdate(const unsigned long deltaMs)
{
	HumanView::VOnUpdate(deltaMs);

	if (m_pFreeCameraController)
	{
		m_pFreeCameraController->OnUpdate(deltaMs);
	}
}

void BombastEditorHumanView::VOnAttach(GameViewId vid, ActorId aid)
{
	HumanView::VOnAttach(vid, aid);
}

bool BombastEditorHumanView::VLoadGameDelegate(rapidxml::xml_node<>* pLevelData)
{
	if (!HumanView::VLoadGameDelegate(pLevelData))
	{
		return false;
	}

	m_pFreeCameraController = BE_NEW MovementController(m_pCamera, 90, 0, true);
	m_pCamera->ClearViewTarget();

	m_pKeyboardHandler = m_pFreeCameraController;
	m_pMouseHandler = m_pFreeCameraController;

	m_pScene->VOnRestore();
	return true;
}