#include "InputCore.h"

InputCore::InputCore()
{
	m_pDirectInput = 0;
	m_pKeyboard = 0;
	m_pMouse = 0;
}

InputCore::InputCore(const InputCore& other)
{
}

InputCore::~InputCore()
{
}

bool InputCore::Initialize(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT result;

	m_mouseX = 0;
	m_mouseY = 0;

	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pKeyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	result = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result)) 
	{
		return false;
	}

	result = m_pMouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputCore::Shutdown()
{
	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		SAFE_RELEASE(m_pMouse);
	}

	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		SAFE_RELEASE(m_pKeyboard);
	}

	if (m_pDirectInput)
	{
		SAFE_RELEASE(m_pDirectInput);
	}
}

bool InputCore::Frame()
{
	bool result;

	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	ProcessInput();

	return true;
}

bool InputCore::ReadKeyboard()
{
	HRESULT result;

	result = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_pKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputCore::ReadMouse()
{
	HRESULT result;

	result = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_pMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputCore::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0)
	{
		m_mouseX = 0;
	}

	if (m_mouseY < 0) 
	{
		m_mouseY = 0;
	}
	/*
	if (m_mouseX > SCREEN_WIDTH) 
	{
		m_mouseX = SCREEN_WIDTH;
	}

	if (m_mouseY > SCREEN_HEIGHT)
	{
		m_mouseY = SCREEN_HEIGHT;
	}*/
}

bool InputCore::IsKeyPressed(int key)
{
	if (m_keyboardState[key] & 0x80)
	{
		return true;
	}

	return false;
}

void InputCore::GetMouseLocation(int& posX, int& posY)
{
	posX = m_mouseX;
	posY = m_mouseY;
}