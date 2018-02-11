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
	bool IsKeyPressed(const BYTE Key) const;
	// returns true If key is down
	bool IsKeyHeld(const BYTE Key) const;
	// returns true first frame is up (Was last down
	bool IsKeyReleased(const BYTE Key) const;
	void GetMouseAxis(float& axisX, float& axisY) const;

private:
	void ProcessMessage(AppMsg msg);

	void ProcessMouseAxis(const Point& pos, HWND hWnd);

private:
	std::bitset<256> m_heldKeyboardState;
	std::bitset<256> m_downKeyboardState;
	std::bitset<256> m_upKeyboardState;

	std::queue<AppMsg> m_messageQueue;

	float m_mouseAxisX, m_mouseAxisY;
};