#include "LuaResource.h"
#include "../Bombast/BombastApp.h"
#include <fstream>

LuaResourceExtraData::LuaResourceExtraData()
{
	m_pLuaScript = 0;
}

void LuaResourceExtraData::Shutdown()
{
	SAFE_DELETE(m_pLuaScript);
}

LuaScript* LuaResourceExtraData::GetScript()
{
	return m_pLuaScript;
}

bool LuaResourceExtraData::LoadScript(char* pRawBuffer, unsigned int rawSize, std::string filename)
{
	bool result;

	m_pLuaScript = BE_NEW LuaScript();
	if(!m_pLuaScript)
	{
		return false;
	}

	result = m_pLuaScript->Initialize(pRawBuffer, rawSize, filename);
	if(!result)
	{
		return false;
	}

	return true;
}

bool LuaResourceLoader::VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle)
{
	bool result;

	if (rawSize <= 0)
	{
		return false;
	}

	LuaResourceExtraData* pExtraData = BE_NEW LuaResourceExtraData();
	pExtraData->LoadScript(rawBuffer, rawSize, handle->GetName());
	
	SAFE_DELETE_ARRAY(rawBuffer);

	handle->SetExtra(pExtraData);

	return true;
}

IResourceLoader* CreateLuaResourceLoader()
{
	return BE_NEW LuaResourceLoader();
}

LuaScript* LuaResourceLoader::LoadAndReturnLuaScript(const char* resourceString)
{
	Resource resource(resourceString);
	ResourceHandle* pResourceHandle = BombastApp::GetGameInstance()->m_pResourceCache->GetHandle(&resource);
	LuaResourceExtraData* pExtraData = (LuaResourceExtraData*)(pResourceHandle->GetExtra());
	
	return pExtraData->GetScript();
}