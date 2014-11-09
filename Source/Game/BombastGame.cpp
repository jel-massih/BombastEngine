#include "BombastGame.h"
#include "../Bombast/BombastApp.h"


BombastGame::BombastGame()
{
	m_pInputCore = 0;
	m_lastTime = GetTickCount();
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
	if (m_pInputCore)
	{
		m_pInputCore->Shutdown();
		SAFE_DELETE(m_pInputCore);
	}
}

bool BombastGame::Frame()
{
	bool result;

	DWORD current_time = GetTickCount();
	DWORD delta_time = current_time - m_lastTime;
	m_lastTime = current_time;

	double elapsed_ms = (double)delta_time / 1000;

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