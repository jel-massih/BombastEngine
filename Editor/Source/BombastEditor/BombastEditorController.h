#pragma once

#include "BombastEngineStd.h"
#include "Bombast\interfaces.h"

class SceneNode;

class BombastEditorController : public IMouseHandler, public IKeyboardHandler
{
public:
	BombastEditorController();
	void OnUpdate(const float deltaMs);

protected:
	BYTE m_bKey[256];

public:
	virtual bool VOnMouseMove(const Point& mousePos, const int radius);
	virtual bool VOnMouseDown(const Point& mousePos, const int radius, const std::string& buttonName);
	virtual bool VOnMouseUp(const Point& mousePos, const int radius, const std::string& buttonName);

	bool VOnKeyDown(const BYTE c);
	bool VOnKeyUp(const BYTE c);
};