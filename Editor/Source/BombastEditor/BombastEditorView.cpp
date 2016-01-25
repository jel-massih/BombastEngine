#include "BombastEditorView.h"
#include "Bombast/interfaces.h"
#include "Graphics3D/MovementController.h"
#include "Graphics3D\RenderNodes.h"

BombastEditorHumanView::BombastEditorHumanView(IRenderer* renderer) : HumanView(renderer)
{
	m_pFreeCameraController = 0;
	m_pGrid = nullptr;
	m_bShowUI = true;
	m_bShowGrid = true;
}

BombastEditorHumanView::~BombastEditorHumanView()
{
	SAFE_DELETE(m_pFreeCameraController);
}

void BombastEditorHumanView::VOnUpdate(const float deltaMs)
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
	m_pCamera->SetPosition(Vec3(-6, 4, -3));

	m_pFreeCameraController = BE_NEW MovementController(m_pCamera, XMConvertToRadians(-20), XMConvertToRadians(40), true);

	m_pKeyboardHandler = m_pFreeCameraController;
	m_pMouseHandler = m_pFreeCameraController;
	m_pCamera->ClearViewTarget();
	m_pCamera->ClearFollowTarget();
	SetCapture(g_pApp->GetHwnd());

	m_pGrid = BE_NEW D3D11GridNode(INVALID_ACTOR_ID, nullptr, &Mat4x4::g_Identity);
	m_pScene->AddChild(INVALID_ACTOR_ID, m_pGrid);
	m_pGrid->SetVisible(m_bShowGrid);

	m_pScene->VOnRestore();

	return true;
}