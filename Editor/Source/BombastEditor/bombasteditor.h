#pragma once

#include "Bombast\BombastApp.h"
#include "Bombast\CoreGameLogic.h"
#include "../Application/MessageHandler.h"

class BombastEditorMain;

class BombastEditorApp : public BombastApp
{
public:
	void OnIdleThread();
	void InitEditorWindow();

public:
	virtual TCHAR *VGetGameTitle() { return _T("Bombast Editor"); }

protected:
	virtual CoreGameLogic *VCreateGameAndView();

private:
	MessageHandler m_messageHandler;
	BombastEditorMain* m_mainEditor;
};