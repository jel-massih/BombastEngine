#include "InputCore.h"

const float MOUSE_AXIS_ZENITH = 0.001f;

InputCore::InputCore()
{
	m_heldKeyboardState.reset();
	m_downKeyboardState.reset();
	m_upKeyboardState.reset();
}

InputCore::~InputCore()
{
}

void InputCore::EnqueueMessage(AppMsg msg)
{
	m_messageQueue.push(msg);
}

void InputCore::Frame()
{
	m_downKeyboardState.reset();
	m_upKeyboardState.reset();
	m_downKeyboardState.reset();

	m_mouseAxisX = 0.f;
	m_mouseAxisY = 0.f;

	while (!m_messageQueue.empty())
	{
		AppMsg msg = m_messageQueue.front();
		ProcessMessage(msg);
		m_messageQueue.pop();
	}
}

void InputCore::ProcessMessage(AppMsg msg)
{
	LRESULT result = 0;
	switch (msg.m_uMsg)
	{
	case WM_KEYDOWN:
		//Check if first down
		if ((HIWORD(msg.m_lParam) & KF_REPEAT) == 0)
		{
			m_downKeyboardState[(BYTE)msg.m_wParam] = 1;
		}
		break;

	case WM_KEYUP:
		m_upKeyboardState[(BYTE)msg.m_wParam] = 1;
		break;

	case WM_MOUSEMOVE:

		//Only process mouse event if window is active
		if (GetActiveWindow() == msg.m_hWnd)
		{
			ProcessMouseAxis(Point(GET_X_LPARAM(msg.m_lParam), GET_Y_LPARAM(msg.m_lParam)), msg.m_hWnd);
		}
		break;

	case WM_LBUTTONDOWN:
		//	SetCapture(msg.m_hWnd);
			//result = m_pMouseHandler->VOnMouseDown(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
		break;

	case WM_LBUTTONUP:
		//	SetCapture(NULL);
			//result = m_pMouseHandler->VOnMouseUp(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
		break;

	case WM_RBUTTONDOWN:
		//	SetCapture(msg.m_hWnd);
		//	result = m_pMouseHandler->VOnMouseDown(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
		break;

	case WM_RBUTTONUP:
		//	SetCapture(NULL);
		//	result = m_pMouseHandler->VOnMouseUp(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
		break;
	}
}

void InputCore::ProcessMouseAxis(const Point& pos, HWND hWnd)
{
	Point screenSize = g_pApp->GetScreenSize();
	const int mouseDeltaX = pos.GetX() - (screenSize.GetX() / 2);
	const int mouseDeltaY = pos.GetY() - (screenSize.GetY() / 2);
	POINT pt;
	pt.x = (screenSize.GetX() / 2);
	pt.y = (screenSize.GetY() / 2);
	ClientToScreen(g_pApp->GetHwnd(), &pt);
	SetCursorPos(pt.x, pt.y);

	float xMovement = (float)(mouseDeltaX * 0.1);
	float yMovement = (float)(mouseDeltaY * 0.1);

	static const float axisMin = -20;
	static const float axisMax = 20;

	if (xMovement < axisMin)
	{
		xMovement = axisMin;
	}
	if (xMovement > axisMax)
	{
		xMovement = axisMax;
	}
	if (yMovement < axisMin)
	{
		yMovement = axisMin;
	}
	if (yMovement > axisMax)
	{
		yMovement = axisMax;
	}

	m_mouseAxisX = (2 * (xMovement - axisMin) / (axisMax - axisMin)) - 1;
	m_mouseAxisY = (2 * (yMovement - axisMin) / (axisMax - axisMin)) - 1;

	if (m_mouseAxisX < MOUSE_AXIS_ZENITH && m_mouseAxisX > -MOUSE_AXIS_ZENITH)
	{
		m_mouseAxisX = 0.f;
	}
	if (m_mouseAxisY < MOUSE_AXIS_ZENITH && m_mouseAxisY > -MOUSE_AXIS_ZENITH)
	{
		m_mouseAxisY = 0.f;
	}
}

void InputCore::GetMouseAxis(float& axisX, float& axisY) const
{
	axisX = m_mouseAxisX;
	axisY = m_mouseAxisY;
}

bool InputCore::IsKeyPressed(BYTE key) const
{
	return m_downKeyboardState[key];
}

bool InputCore::IsKeyHeld(BYTE key) const
{
	return m_heldKeyboardState[key];
}

bool InputCore::IsKeyReleased(BYTE key) const
{
	return m_upKeyboardState[key];
}