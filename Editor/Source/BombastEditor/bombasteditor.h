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
	void SetEditor(BombastEditorMain* pEditor);

	void OnViewportKeyDown(BYTE keyCode);
	void OnViewportKeyUp(BYTE keyCode);
	void OnViewportMouseDown(const int posX, const int posY, const std::string& keyName);
	void OnViewportMouseUp(const int posX, const int posY, const std::string& keyName);

	void EditorQuit();

public:
	virtual TCHAR *VGetGameTitle() { return _T("Bombast Editor"); }

protected:
	virtual CoreGameLogic *VCreateGameAndView();

private:
	MessageHandler m_messageHandler;
	BombastEditorMain* m_mainEditor;
};