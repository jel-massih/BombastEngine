#include "EventManager.h"
#include "../Bombast/BombastApp.h"
#include "../BombastEngineStd.h"
#include "..\Utilities\templates.h"

static IEventManager* g_pEventMgr = NULL;

IEventManager* IEventManager::Get()
{
	BE_ASSERT(g_pEventMgr);
	return g_pEventMgr;
}

IEventManager::IEventManager(const char* pName, bool setAsGlobal)
{
	if (setAsGlobal)
	{
		if (g_pEventMgr)
		{
			BE_ERROR("Attempting to create 2 global event managers! Old one will be destroyed");
			delete g_pEventMgr;
		}

		g_pEventMgr = this;
	}
}

IEventManager::~IEventManager()
{
	if (g_pEventMgr == this)
		g_pEventMgr = NULL;
}