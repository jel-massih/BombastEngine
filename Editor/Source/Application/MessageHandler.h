#pragma once
#include <qobject.h>
#include <qtimer.h>
class BombastEditorApp;

class MessageHandler : public QObject
{
	Q_OBJECT

public:
	~MessageHandler();

	void Init(BombastEditorApp* pParentApp);

public slots:
		void AppIdle();

private:
	QTimer m_pTimer;
	BombastEditorApp* m_pEditorApp;
};