#ifndef GAME_H
#define GAME_H

#include "../BombastEngineStd.h"
#include "../Graphics2D/BitmapClass.h"
#include "../Bombast/EntitiesManager.h"

class BombastGame {
public:
	BombastGame();
	~BombastGame();
	BombastGame(const BombastGame& other);

	bool Initialize();
	void Shutdown();

private:
	EntitiesManager* m_pEntitiesManager;

	BitmapClass* m_pBackgroundBitmap;
	BitmapClass* m_pLeftPaddleBitmap;
	BitmapClass* m_pRightPaddleBitmap;
};

#endif