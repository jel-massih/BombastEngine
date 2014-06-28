#include "BombastGame.h"
#include "../Bombast/BombastApp.h"


BombastGame::BombastGame()
{
	m_pInputCore = 0;
	m_pEntitiesManager = 0;
	m_pBackgroundBitmap = 0;
	m_pLeftPaddleBitmap = 0;
	m_pRightPaddleBitmap = 0;
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

	BombastApp* pBombastApp = BombastApp::GetGameInstance();
	m_pEntitiesManager = pBombastApp->GetEntitiesManager();

	m_pBackgroundBitmap = BE_NEW BitmapClass();
	if (!m_pBackgroundBitmap) 
	{
		return false;
	}

	result = m_pBackgroundBitmap->Initialize(pBombastApp->GetGraphicsManager()->GetD3DClass()->GetDevice(), L"../../Game/Data/dirt01.dds", SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!result) 
	{
		return false;
	}

	m_pEntitiesManager->RegisterBitmap(m_pBackgroundBitmap);

	m_pLeftPaddleBitmap = BE_NEW BitmapClass();
	if (!m_pLeftPaddleBitmap)
	{
		return false;
	}

	result = m_pLeftPaddleBitmap->Initialize(pBombastApp->GetGraphicsManager()->GetD3DClass()->GetDevice(), L"../../Game/Data/paddle.dds", 10, 200);
	if (!result)
	{
		return false;
	}

	m_pEntitiesManager->RegisterBitmap(m_pLeftPaddleBitmap);

	m_pRightPaddleBitmap = BE_NEW BitmapClass();
	if (!m_pRightPaddleBitmap)
	{
		return false;
	}

	result = m_pRightPaddleBitmap->Initialize(pBombastApp->GetGraphicsManager()->GetD3DClass()->GetDevice(), L"../../Game/Data/paddle.dds", 10, 200);
	if (!result)
	{
		return false;
	}

	m_pEntitiesManager->RegisterBitmap(m_pRightPaddleBitmap);

	m_pBackgroundBitmap->SetPosition(0, 0);
	m_pLeftPaddleBitmap->SetPosition(100, SCREEN_HEIGHT / 2 - m_pLeftPaddleBitmap->GetBitmapHeight() / 2);
	m_pRightPaddleBitmap->SetPosition(SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2 - m_pRightPaddleBitmap->GetBitmapHeight()/2);

	return true;
}

void BombastGame::Shutdown()
{
	if (m_pRightPaddleBitmap) {
		m_pEntitiesManager->DeRegisterBitmap(m_pRightPaddleBitmap);
		m_pRightPaddleBitmap->Shutdown();
		SAFE_DELETE(m_pRightPaddleBitmap);
	}
	
	if (m_pLeftPaddleBitmap) {
		m_pEntitiesManager->DeRegisterBitmap(m_pLeftPaddleBitmap);
		m_pLeftPaddleBitmap->Shutdown();
		SAFE_DELETE(m_pLeftPaddleBitmap);
	}

	if (m_pBackgroundBitmap) {
		m_pEntitiesManager->DeRegisterBitmap(m_pBackgroundBitmap);
		m_pBackgroundBitmap->Shutdown();
		SAFE_DELETE(m_pBackgroundBitmap);
	}

	m_pEntitiesManager = 0;

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

	if (m_pInputCore->IsKeyPressed(DIK_W))
	{
		m_pLeftPaddleBitmap->Translate(0, -300 * elapsed_ms);
	}

	if (m_pInputCore->IsKeyPressed(DIK_S))
	{
		m_pLeftPaddleBitmap->Translate(0, 300 * elapsed_ms);
	}

	if (m_pInputCore->IsKeyPressed(DIK_UP))
	{
		m_pRightPaddleBitmap->Translate(0, -300 * elapsed_ms);
	}

	if (m_pInputCore->IsKeyPressed(DIK_DOWN))
	{
		m_pRightPaddleBitmap->Translate(0, 300 * elapsed_ms);
	}

	//_RPT1(0, "height: %d\n", m_pLeftPaddleBitmap->)
	

	return true;
}