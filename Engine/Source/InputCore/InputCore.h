#pragma once

#include "../BombastEngineStd.h"

#include <bitset>
#include <queue>

class InputCore
{
public:
	InputCore();
	~InputCore();

	void EnqueueMessage(AppMsg msg);
	void Frame();

	// returns true first frame key is pressed (Was last up)
	bool IsKeyPressed(BYTE Key);
	// returns true If key is down
	bool IsKeyHeld(BYTE Key);
	// returns true first frame is up (Was last down
	bool IsKeyReleased(BYTE Key);
	void GetMouseLocation(int& posX, int& posY);

private:
	void ProcessMessage(AppMsg msg);

private:
	std::bitset<256> m_heldKeyboardState;
	std::bitset<256> m_downKeyboardState;
	std::bitset<256> m_upKeyboardState;

	std::queue<AppMsg> m_messageQueue;

	int m_mouseX, m_mouseY;
};