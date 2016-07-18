#include "LuaCoreManager.h"
#include "../Bombast/BombastApp.h"
#include "../Resources/LuaResource.h"
#include "LuaExports.h"

#include <iostream>

static int l_print_intercept(lua_State* L)
{
	int nargs = lua_gettop(L);
	std::string printString;
	for (int i = 1; i <= nargs; i++)
	{
		printString += lua_tostring(L, i);
	}

	BE_LOG_SCRIPT(printString.c_str());

	return 0;
}

static const struct luaL_Reg interceptLib[] = {
	{ "print", l_print_intercept },
	{ nullptr, nullptr }
};

LuaCoreManager::~LuaCoreManager()
{
	if (L)
	{
		lua_close(L);
	}
}

bool LuaCoreManager::Initialize()
{
	L = luaL_newstate();

	//Load ALL THE LIBS!!!!!!!!!
	luaL_openlibs(L);
	
	//Add intercept
	lua_getglobal(L, "_G");
	luaL_setfuncs(L, interceptLib, 0);
	lua_pop(L, 1);

	if (!RegisterLoader())
	{
		BE_ERROR("Failed to Register Custom Require Looader");
		return false;
	}

	return true;
}

bool LuaCoreManager::LoadScript(std::string filename)
{
	LuaResourceLoader::LoadLuaScript(filename.c_str());

	return true;
}

bool LuaCoreManager::LoadScriptFromBuffer(char* rawBuffer, int rawSize, std::string filename)
{
	int error = luaL_loadbuffer(L, rawBuffer, rawSize, filename.c_str());
	if (error)
	{
		BE_ERROR("Script Failed To Load: %s", filename.c_str());
		return false;
	}

	error = lua_pcall(L, 0, 0, 0);
	if (error)
	{
		std::string errorMessage = lua_tostring(L, -1);
		BE_ERROR("Script Failed To Run: [%s] ErrorCode: %d ErrorMessage: %s", filename.c_str(), error, errorMessage.c_str());
		//Load Was successful so still return true
	}

	return true;
}

void LuaCoreManager::PrintError(const std::string& variableName, const std::string& reason)
{
	BE_ERROR("Cant Get [%s]. %s", variableName.c_str(), reason.c_str());
}

bool LuaCoreManager::RegisterLoader()
{
	/**Register New package.loader**/
	if (lua_gettostack("package.searchers"))
	{
		lua_remove(L, -2);

		//Replace first loader with custom loader
		lua_pushinteger(L, 1);
		lua_pushcfunction(L, InternalLuaExports::lua_loader);
		lua_rawset(L, -3);
		lua_pop(L, 1);
		return true;
	}

	return false;
}