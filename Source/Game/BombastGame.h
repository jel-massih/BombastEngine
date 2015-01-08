#pragma once

#include "../BombastEngineStd.h"

class InputCore;

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
};