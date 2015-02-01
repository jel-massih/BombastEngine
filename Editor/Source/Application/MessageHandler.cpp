#include "MessageHandler.h"
#include "../BombastEditor/BombastEditor.h"

MessageHandler::~MessageHandler()
{
	m_pTimer.disconnect();
}

void MessageHandler::Init(BombastEditorApp* pParentApp)
{
	m_pEditorApp = pParentApp;

	connect(&m_pTimer, SIGNAL(timeout()), this, SLOT(AppIdle()));
	m_pTimer.start(0);
}

void MessageHandler::AppIdle()
{
	m_pEditorApp->OnIdleThread();
}