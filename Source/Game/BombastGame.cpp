#include "BombastGame.h"
#include "../Bombast/BombastApp.h"


BombastGame::BombastGame()
{
	m_pEntitiesManager = 0;
	m_pBackgroundBitmap = 0;
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

	return true;
}

void BombastGame::Shutdown()
{
	if (m_pBackgroundBitmap) {
		m_pEntitiesManager->DeRegisterBitmap(m_pBackgroundBitmap);
		m_pBackgroundBitmap->Shutdown();
		SAFE_DELETE(m_pBackgroundBitmap);
	}

	m_pEntitiesManager = 0;
}