#include "LuaCoreManager.h"

LuaCoreManager::LuaCoreManager()
{
	m_pLuaState = 0;
}

LuaCoreManager::LuaCoreManager(const LuaCoreManager& other)
{
}

LuaCoreManager::~LuaCoreManager()
{
}

bool LuaCoreManager::Initialize()
{
	m_pLuaState = luaL_newstate();

	static const luaL_Reg lualibs[] =
	{
		{"base", luaopen_base},
		{NULL, NULL}
	};

	const luaL_Reg* lib = lualibs;

	for(; lib->func != NULL; lib++)
    {
        lib->func(m_pLuaState);
        lua_settop(m_pLuaState, 0);
    }

	return true;
}

void LuaCoreManager::Shutdown()
{
	lua_close(m_pLuaState);
}

void LuaCoreManager::RunScript(const char* fileName)
{
	luaL_dofile(m_pLuaState, fileName);
}