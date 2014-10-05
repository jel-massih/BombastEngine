#include "LuaCoreManager.h"

#include "../Resources/LuaResource.h"

LuaCoreManager::LuaCoreManager()
{
}

LuaCoreManager::LuaCoreManager(const LuaCoreManager& other)
{
}

LuaCoreManager::~LuaCoreManager()
{
}

bool LuaCoreManager::Initialize()
{
	return true;
}

void LuaCoreManager::Shutdown()
{
	m_scripts.clear();
}

bool LuaCoreManager::LoadScript(std::string filename)
{
	LuaScript* script = LuaResourceLoader::LoadAndReturnLuaScript(filename.c_str());
	if(script)
	{
		m_scripts.insert(std::make_pair(filename, script));
		return true;
	}

	return false;
}

bool LuaCoreManager::RunScript(std::string filename)
{
	for (auto it = m_scripts.begin(); it != m_scripts.end(); ++it)
	{
		if(it->first == filename)
		{
			return it->second->RunScript();
		}
	}

	return false;
}