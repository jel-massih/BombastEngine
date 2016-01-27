#pragma once
#include "Bombast\CoreGameLogic.h"

class BombastEditorHumanView;

class BombastEditorLogic : public CoreGameLogic
{
public:
	BombastEditorLogic();
	virtual ~BombastEditorLogic();

	void VChangeState(CoreGameState newState);

	void OnKeyDown(BYTE keyCode);
	void OnKeyUp(BYTE keyCode);

	void OnMouseDown(const Point& pos, const std::string& keyName);
	void OnMouseUp(const Point& pos, const std::string& keyName);

protected:
	BombastEditorHumanView* m_pActiveHumanView; //Weak Ptr
};