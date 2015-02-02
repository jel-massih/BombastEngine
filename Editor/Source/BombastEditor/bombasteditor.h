#pragma once

#include "Bombast\BombastApp.h"
#include "Bombast\CoreGameLogic.h"
#include "../Application/MessageHandler.h"

class BombastEditorMain;

extern BombastEditorApp g_BombastEditorApp;

class BombastEditorApp : public BombastApp
{
public:
	~BombastEditorApp();

	void OnIdleThread();
	void InitEditorWindow();

	void OnViewportKeyDown(BYTE keyCode);
	void OnViewportKeyUp(BYTE keyCode);

	void EditorQuit();

public:
	virtual TCHAR *VGetGameTitle() { return _T("Bombast Editor"); }

protected:
	virtual CoreGameLogic *VCreateGameAndView();

private:
	MessageHandler m_messageHandler;
	BombastEditorMain* m_mainEditor;
};