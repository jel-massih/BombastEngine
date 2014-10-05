#include "LuaScript.h"
#include "../Bombast/BombastApp.h"

LuaScript::LuaScript(const std::string& filename)
{
	m_filename = filename;
	L = luaL_newstate();
	if(luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0)) 
	{
		std::wstring s(s2ws("Script Failed To Load: " + filename));
		OutputDebugString(s.c_str());
	}
}

LuaScript::LuaScript()
{
	L = luaL_newstate();

    static const luaL_Reg lualibs[] =
    {
        {"base", luaopen_base},
        {NULL, NULL}
    };

    const luaL_Reg* lib = lualibs;

    for(; lib->func != NULL; lib++)
    {
        lib->func(L);
        lua_settop(L, 0);
    }	
}

bool LuaScript::Initialize(const char* rawBuffer, unsigned int rawSize, std::string filename)
{
	m_filename = filename;


	int error = luaL_loadbuffer(L, rawBuffer, rawSize, filename.c_str());
	if(error) 
	{
		BE_ERROR("Script Failed To Load: " + filename);
		return false;
	}

	return true;
}

bool LuaScript::RunScript()
{
	int error = lua_pcall(L, 0, 0, 0);
	if(error)
	{
		BE_ERROR("Script Failed To Run: [" + m_filename + "] ErrorCode: " + std::to_string(error));
		return false;
	}

	return true;
}

LuaScript::~LuaScript()
{
	if(L)
	{
		lua_close(L);
	}
}

void LuaScript::PrintError(const std::string& variableName, const std::string& reason)
{
	BE_ERROR("Cant Get [" + variableName + "]. " + reason);
}