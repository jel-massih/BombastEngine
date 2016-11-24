#include "BombastEditorGlobalFunctions.h"
#include "Utilities/String.h"

#include "Actor/Actor.h"
#include "Events/Events.h"


int InitializeBombastProject(int *instancePtrAddress, int *hPrevInstancePtrAddress, int *hWndPtrAddress, int nCmdShow, int screenWidth, int screenHeight, BSTR projectPath)
{
	HINSTANCE hInstance = (HINSTANCE)instancePtrAddress;
	HINSTANCE hPrevInstance = (HINSTANCE)hPrevInstancePtrAddress;
	HWND hWnd = (HWND)hWndPtrAddress;
	WCHAR *lpCmdLine = L"";

	bool bConsoleEnabled = false;

	//Setup Checks for memory leaks
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	//Used at Program Init to force leak check before exit
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(tmpDbgFlag);

	g_pApp->m_options.Init((ROOT_GAME_PATH + "Options.xml").c_str(), lpCmdLine);

	//Add Project Directory
	g_pApp->m_options.m_additionalContentDirectories += (ws2s(projectPath) + "\\Assets/,");

	BELogger::Init(g_pApp->m_options.m_bDebugConsoleEnabled, g_pApp->m_options.m_debugLogPath.c_str(), g_pApp->m_options.m_debugLogName.c_str());

	if (!g_pApp->InitInstance(hInstance, hWnd, screenWidth, screenHeight))
	{
		return FALSE;
	}

	return TRUE;
}

void WndProc(int *hWndPtrAddress, int msg, int wParam, int lParam)
{
	HWND hWnd = (HWND)hWndPtrAddress;
	g_pApp->MsgProc(hWnd, msg, WPARAM(wParam), LPARAM(lParam));
}


void RenderFrame()
{
	g_pApp->Render();
}

void UpdateEngine()
{
	g_pApp->ExternalEngineUpdate();
}

int Shutdown()
{
	g_pApp->ShutDown();
	return 1;
}

bool OpenProject(BSTR fullProjectPath)
{
	std::string projectPath = ws2s(std::wstring(fullProjectPath, SysStringLen(fullProjectPath)));

	BombastEditorLogic* pEditorLogic = (BombastEditorLogic*)g_pApp->m_pGame;
	pEditorLogic->SetActiveProjectPath(projectPath);
	
	return true;
}

bool OpenLevel(BSTR levelResourceName)
{
	BombastEditorLogic* pEditorLogic = (BombastEditorLogic*)g_pApp->m_pGame;

	if (pEditorLogic)
	{
		g_pApp->m_options.m_level = ws2s(levelResourceName);
		pEditorLogic->VChangeState(CGS_LoadingGameEnvironment);
	}

	return true;
}

int GetActorCount()
{
	BombastEditorLogic* pGame = static_cast<BombastEditorLogic*>(g_pApp->m_pGame);
	return pGame->GetActorCount();
}

void GetActorList(int* actorIdArrayPtr, int actorCount)
{
	BombastEditorLogic* pEditorLogic = (BombastEditorLogic*)g_pApp->m_pGame;
	if (pEditorLogic)
	{
		ActorMap::const_iterator itr;
		int actorArrayIndex;
		for (itr = pEditorLogic->GetActorMap().begin(), actorArrayIndex = 0;
			itr != pEditorLogic->GetActorMap().end() && actorArrayIndex < actorCount; itr++, actorArrayIndex++)
		{
			ActorId actorId = itr->first;
			actorIdArrayPtr[actorArrayIndex] = actorId;
		}
	}
}

void GetActorXml(int *actorXmlPtr, ActorId actorId)
{
	Actor* pActor = g_pApp->m_pGame->VGetActor(actorId);
	if (!pActor)
	{
		return;
	}
	std::string xml = pActor->ToXML();

	strncpy_s(reinterpret_cast<char *>(actorXmlPtr), xml.length() + 1, xml.c_str(), xml.length());
}

int GetActorXmlSize(ActorId actorId)
{
	Actor* pActor = g_pApp->m_pGame->VGetActor(actorId);
	if (!pActor)
	{
		return 0;
	}
	std::string xml = pActor->ToXML();
	return xml.length();
}

int CreateActor(BSTR actorResourceXml)
{
	std::string actorResource = ws2s(std::wstring(actorResourceXml, SysStringLen(actorResourceXml)));
	Actor* pActor = g_pApp->m_pGame->VCreateActor(actorResource, NULL);
	if (!pActor)
		return INVALID_ACTOR_ID;

	EventDataPtr pEvent(BE_NEW EvtData_New_Actor(pActor->GetId()));
	IEventManager::Get()->VQueueEvent(pEvent);
	return pActor->GetId();
}

void ModifyActor(BSTR newActorXml)
{
	std::string actorModificationXML = ws2s(std::wstring(newActorXml, SysStringLen(newActorXml)));
	rapidxml::xml_document<> doc;
	doc.parse<0>(&actorModificationXML[0]);
	rapidxml::xml_node<> *pRoot = doc.first_node();
	if (!pRoot)
		return;

	std::string actorId = pRoot->first_attribute("id")->value();

	g_pApp->m_pGame->VModifyActor(atoi(actorId.c_str()), pRoot);
}

void DestroyActor(ActorId actorId)
{
	g_pApp->m_pGame->VDestroyActor(actorId);
}