#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include "../BombastEngineStd.h"
#include <dinput.h>

class InputCore
{
public:
	InputCore();
	InputCore(const InputCore& other);
	~InputCore();

	bool Initialize(HINSTANCE hInstance, HWND hWnd);
	void Shutdown();

	bool Frame();

	bool IsKeyPressed(int Key);
	void GetMouseLocation(int& posX, int& posY);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_pDirectInput;
	IDirectInputDevice8* m_pKeyboard;
	IDirectInputDevice8* m_pMouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_mouseX, m_mouseY;
};