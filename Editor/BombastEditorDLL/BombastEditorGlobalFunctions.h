#pragma once

#define DllExport _declspec(dllexport)

#include "BombastEditor.h"

extern "C" DllExport int InitializeBombastProject(int *instancePtrAddress, int *hPrevInstancePtrAddress, int *hWndPtrAddress, int nCmdShow, int screenWidth, int screenHeight, BSTR projectPath);
extern "C" DllExport void WndProc(int *hWndPtrAddress, int msg, int wParam, int lParam);
extern "C" DllExport void RenderFrame();
extern "C" DllExport int Shutdown();

extern "C" DllExport bool OpenProject(BSTR projectPath);
extern "C" DllExport bool OpenLevel(BSTR levelResourceName);

extern "C" DllExport int GetActorCount();
extern "C" DllExport void GetActorList(int* actorIdArrayPtr, int size);
extern "C" DllExport void GetActorXml(int *actorXmlPtr, ActorId actorId);
extern "C" DllExport int GetActorXmlSize(ActorId actorId);

extern "C" DllExport int CreateActor(BSTR actorResourceXml);
extern "C" DllExport void ModifyActor(BSTR newActorXml);
extern "C" DllExport void DestroyActor(ActorId actorId);
