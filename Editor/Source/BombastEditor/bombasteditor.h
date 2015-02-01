#pragma once

#include "Bombast\CoreGameLogic.h"
#include "Bombast\BombastApp.h"

class BombastEditorApp : public BombastApp
{
protected:
	virtual CoreGameLogic* VCreateGameAndView();

public:
	virtual TCHAR *VGetGameTitle() { return _T("Bombast Editor"); }
};


class BombastEditorLogic : public CoreGameLogic
{
protected:

public:
	BombastEditorLogic();
	virtual ~BombastEditorLogic();
};