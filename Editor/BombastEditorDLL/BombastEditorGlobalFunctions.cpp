#include "BombastEditorGlobalFunctions.h"
#include "Utilities/String.h"

int EditorMain(int *instancePtrAddress, int *hPrevInstancePtrAddress, int *hWndPtrAddress, int nCmdShow, int screenWidth, int screenHeight)
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

bool OpenLevel(BSTR fullLevelPath)
{
	std::string levelFile = ws2s(std::wstring(fullLevelPath, SysStringLen(fullLevelPath)));
	BombastEditorLogic* pEditorLogic = (BombastEditorLogic*)g_pApp->m_pGame;

	if (pEditorLogic)
	{
		std::string assetsDir = "\\Assets\\";
		int projDirLength = pEditorLogic->GetProjectDirectory().length() + assetsDir.length();
		g_pApp->m_options.m_level = levelFile.substr(projDirLength, levelFile.length() - projDirLength);
		pEditorLogic->VChangeState(CGS_LoadingGameEnvironment);
	}

	return true;
}

int GetActorCount()
{
	BombastEditorLogic* pGame = static_cast<BombastEditorLogic*>(g_pApp->m_pGame);
	return pGame->GetActorCount();
}

void GetActorList(int* actorIdArrayPtr, int size)
{

}

void GetActorXml(int *actorXmlPtr, ActorId actorId)
{

}