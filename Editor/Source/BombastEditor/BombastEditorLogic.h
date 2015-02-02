#pragma once
#include "Bombast\CoreGameLogic.h"

class BombastEditorLogic : public CoreGameLogic
{
public:
	BombastEditorLogic();
	virtual ~BombastEditorLogic();

	void VChangeState(CoreGameState newState);
};