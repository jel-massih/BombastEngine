#ifndef GAME_H
#define GAME_H

#include "../BombastEngineStd.h"
#include "../Graphics2D/BitmapClass.h"
#include "../Bombast/EntitiesManager.h"
#include "../InputCore/InputCore.h"

class BombastGame {
public:
	BombastGame();
	~BombastGame();
	BombastGame(const BombastGame& other);

	bool Initialize(HWND hWnd, HINSTANCE hInstance);
	void Shutdown();

	bool Frame();

private:
	EntitiesManager* m_pEntitiesManager;

	InputCore* m_pInputCore;

	BitmapClass* m_pBackgroundBitmap;
	BitmapClass* m_pLeftPaddleBitmap;
	BitmapClass* m_pRightPaddleBitmap;
};

#endif