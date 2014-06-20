#include "BombastGame.h"
#include "../Bombast/BombastApp.h"


BombastGame::BombastGame()
{
	m_pEntitiesManager = 0;
}

BombastGame::~BombastGame()
{
}

BombastGame::BombastGame(const BombastGame& other)
{
}

bool BombastGame::Initialize()
{
	BombastApp* pBombastApp = BombastApp::GetGameInstance();
	m_pEntitiesManager = pBombastApp->GetEntitiesManager();
	
	return true;
}

void BombastGame::Shutdown()
{
	m_pEntitiesManager = 0;
}