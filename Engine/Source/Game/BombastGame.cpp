#include "BombastGame.h"
#include "../Bombast/BombastApp.h"
#include "../InputCore/InputCore.h"


BombastGame::BombastGame()
{
	m_pInputCore = 0;
}

BombastGame::~BombastGame()
{
}

BombastGame::BombastGame(const BombastGame& other)
{
}

bool BombastGame::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	bool result;

	m_pInputCore = BE_NEW InputCore();
	result = m_pInputCore->Initialize(hInstance, hWnd);
	if (!result)
	{
		return false;
	}
	return true;
}

void BombastGame::Shutdown()
{
	SAFE_DELETE(m_pInputCore);
}

bool BombastGame::Frame()
{
	bool result;

	result = m_pInputCore->Frame();
	if (!result)
	{
		return false;
	}

	//If Escape Pressed, Exit Game
	if (m_pInputCore->IsKeyPressed(DIK_ESCAPE))
	{
		return false;
	}

	return true;
}