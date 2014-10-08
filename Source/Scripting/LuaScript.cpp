#include "LuaScript.h"
#include "../Bombast/BombastApp.h"

int lua_loader(lua_State* L)
{
	const char* file = lua_tostring(L, 1);
	{
		Resource resource(file);
		ResourceHandle* resHandle = BombastApp::GetGameInstance()->m_pResourceCache->GetHandle(&resource);
	}

	return 1;
}

LuaScript::LuaScript()
{
	L = luaL_newstate();

	//Load ALL THE LIBS!!!!!!!!!
	luaL_openlibs(L);

	if(!RegisterLoader())
	{
		BE_ERROR("Failed to Register Custom Require Looader");
	}
}

bool LuaScript::RegisterLoader()
{
	/**Register New package.loader**/
	if(lua_gettostack("package.searchers"))
	{
		lua_remove(L, -2);

		//Replace first loader with custom loader
		lua_pushinteger(L, 1);
		lua_pushcfunction(L, lua_loader);
		lua_rawset(L, -3);
		lua_pop(L, 1);
		return true;
	}
	
	return false;
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