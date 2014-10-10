#include "LuaResource.h"
#include "../Bombast/BombastApp.h"
#include <fstream>

bool LuaResourceExtraData::LoadScript(char* pRawBuffer, unsigned int rawSize, std::string filename)
{
	return BombastApp::GetGameInstance()->GetLuaCoreManager()->LoadScriptFromBuffer(pRawBuffer, rawSize, filename);
}

bool LuaResourceLoader::VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle)
{
	bool result;

	if (rawSize <= 0)
	{
		return false;
	}

	LuaResourceExtraData* pExtraData = BE_NEW LuaResourceExtraData();
	result = pExtraData->LoadScript(rawBuffer, rawSize, handle->GetName());

	if (!result) {
		SAFE_DELETE(pExtraData);
		return false;
	}

	handle->SetExtra(pExtraData);

	return true;
}

IResourceLoader* CreateLuaResourceLoader()
{
	return BE_NEW LuaResourceLoader();
}

void LuaResourceLoader::LoadLuaScript(const char* resourceString)
{
	//Just loading resourcehandle loads the scro[t
	Resource resource(resourceString);
	ResourceHandle* pResourceHandle = BombastApp::GetGameInstance()->m_pResourceCache->GetHandle(&resource);
}