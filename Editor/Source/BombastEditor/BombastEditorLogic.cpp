#include "BombastEditorLogic.h"
#include "Physics/Physics.h"
#include "BombastEditorView.h"

BombastEditorLogic::BombastEditorLogic()
{
	m_pGamePhysics = CreateGamePhysics(g_pApp->m_options.m_physicsEngine);
	m_pActiveHumanView = nullptr;
}

BombastEditorLogic::~BombastEditorLogic()
{
}

void BombastEditorLogic::VChangeState(CoreGameState newState)
{
	CoreGameLogic::VChangeState(newState);

	switch (newState)
	{
	case CGS_WaitingForPlayers:
	{
		BE_ASSERT(m_expectedPlayers == 1);

		for (int i = 0; i < m_expectedPlayers; i++)
		{
			IGameView* playerView = BE_NEW BombastEditorHumanView(g_pApp->GetGraphicsManager()->GetRenderer());
			VAddView(playerView);
			m_pActiveHumanView = (BombastEditorHumanView*)playerView;
			m_humanPlayersAttached++;
		}

		break;
	}

	case CGS_SpawningPlayersActors:
	{
		break;
	}

	case CGS_LoadingGameEnvironment:
	{
		VChangeState(CGS_SpawningPlayersActors);
		break;
	}

	case CGS_Initializing:
	{
		VChangeState(CGS_WaitingForPlayers);
		break;
	}
	}
}

void BombastEditorLogic::OnKeyDown(BYTE keyCode)
{
	m_pActiveHumanView->m_pKeyboardHandler->VOnKeyDown(keyCode);
}

void BombastEditorLogic::OnKeyUp(BYTE keyCode)
{
	m_pActiveHumanView->m_pKeyboardHandler->VOnKeyUp(keyCode);
}