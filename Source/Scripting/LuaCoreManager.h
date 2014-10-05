#ifndef LUA_CORE_MANAGER_H
#define LUA_CORE_MANAGER_H

#include <lua.hpp>

class LuaCoreManager {
public:
	LuaCoreManager();
	LuaCoreManager(const LuaCoreManager& other);
	~LuaCoreManager();

	bool Initialize();
	void Shutdown();

	void RunScript(const char* fileName);

private:
	lua_State* m_pLuaState;
};

#endif