#include "InputCore.h"

InputCore::InputCore()
{
	m_mouseX = 0;
	m_mouseY = 0;

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
		//result = m_pMouseHandler->VOnMouseMove(Point(GET_X_LPARAM(msg.m_lParam), GET_Y_LPARAM(msg.m_lParam)), 1);
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

void InputCore::GetMouseLocation(int& posX, int& posY)
{
	posX = m_mouseX;
	posY = m_mouseY;
}