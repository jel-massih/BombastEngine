#include "BombastEditorController.h"
#include "BombastEditorEvents.h"

BombastEditorController::BombastEditorController()
{
	memset(m_bKey, 0x00, sizeof(m_bKey));
}

void BombastEditorController::OnUpdate(const float deltaMs)
{
}

bool BombastEditorController::VOnMouseMove(const Point & mousePos, const int radius)
{
	return true;
}

bool BombastEditorController::VOnMouseDown(const Point & mousePos, const int radius, const std::string & buttonName)
{
	return true;
}

bool BombastEditorController::VOnMouseUp(const Point & mousePos, const int radius, const std::string & buttonName)
{
	return true;
}

bool BombastEditorController::VOnKeyDown(const BYTE c)
{
	return true;
}

bool BombastEditorController::VOnKeyUp(const BYTE c)
{
	return true;
}
