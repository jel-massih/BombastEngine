#ifndef GAME_H
#define GAME_H

#include "../BombastEngineStd.h"
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
	InputCore* m_pInputCore;

	DWORD m_lastTime;
};

#endif