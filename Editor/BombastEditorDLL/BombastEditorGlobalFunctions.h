#pragma once

#define DllExport _declspec(dllexport)

#include "BombastEditor.h"

extern "C" DllExport int EditorMain(int *instancePtrAddress, int *hPrevInstancePtrAddress, int *hWndPtrAddress, int nCmdShow, int screenWidth, int screenHeight);
extern "C" DllExport void WndProc(int *hWndPtrAddress, int msg, int wParam, int lParam);
extern "C" DllExport void RenderFrame();
extern "C" DllExport int Shutdown();

extern "C" DllExport bool OpenProject(BSTR projectPath);
extern "C" DllExport bool OpenLevel(BSTR fullLevelPath);

extern "C" DllExport int GetActorCount();
extern "C" DllExport void GetActorList(int* actorIdArrayPtr, int size);
extern "C" DllExport void GetActorXml(int *actorXmlPtr, ActorId actorId);