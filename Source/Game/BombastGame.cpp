#include "BombastGame.h"
#include "../Bombast/BombastApp.h"


BombastGame::BombastGame()
{
	m_pEntitiesManager = 0;
	m_pBackgroundBitmap = 0;
	m_pLeftPaddleBitmap = 0;
	m_pRightPaddleBitmap = 0;
}

BombastGame::~BombastGame()
{
}

BombastGame::BombastGame(const BombastGame& other)
{
}

bool BombastGame::Initialize()
{
	bool result;

	BombastApp* pBombastApp = BombastApp::GetGameInstance();
	m_pEntitiesManager = pBombastApp->GetEntitiesManager();

	m_pBackgroundBitmap = BE_NEW BitmapClass();
	if (!m_pBackgroundBitmap) 
	{
		return false;
	}

	result = m_pBackgroundBitmap->Initialize(pBombastApp->GetGraphicsManager()->GetD3DClass()->GetDevice(), SCREEN_WIDTH, SCREEN_HEIGHT, L"../../Game/Data/dirt01.dds", SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!result) 
	{
		return FALSE;
	}

	m_pEntitiesManager->RegisterBitmap(m_pBackgroundBitmap);

	m_pLeftPaddleBitmap = BE_NEW BitmapClass();
	if (!m_pLeftPaddleBitmap)
	{
		return false;
	}

	result = m_pLeftPaddleBitmap->Initialize(pBombastApp->GetGraphicsManager()->GetD3DClass()->GetDevice(), 10, 200, L"../../Game/Data/paddle.dds", 10, 200);
	if (!result)
	{
		return FALSE;
	}

	m_pEntitiesManager->RegisterBitmap(m_pLeftPaddleBitmap);

	m_pRightPaddleBitmap = BE_NEW BitmapClass();
	if (!m_pRightPaddleBitmap)
	{
		return false;
	}

	result = m_pRightPaddleBitmap->Initialize(pBombastApp->GetGraphicsManager()->GetD3DClass()->GetDevice(), 10, 200, L"../../Game/Data/paddle.dds", 10, 200);
	if (!result)
	{
		return FALSE;
	}

	m_pEntitiesManager->RegisterBitmap(m_pRightPaddleBitmap);

	m_pBackgroundBitmap->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	m_pLeftPaddleBitmap->SetPosition(100, 0);
	m_pRightPaddleBitmap->SetPosition(SCREEN_WIDTH - 100 + m_pRightPaddleBitmap->GetBitmapWidth()/2, 0);

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
}