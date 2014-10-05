#ifndef LUA_CORE_MANAGER_H
#define LUA_CORE_MANAGER_H

#include <map>

#include "LuaScript.h"

class LuaCoreManager {
public:
	LuaCoreManager();
	LuaCoreManager(const LuaCoreManager& other);
	~LuaCoreManager();

	bool Initialize();
	void Shutdown();

	bool LoadScript(std::string fileName);
	bool RunScript(std::string fileName);
private:
	std::map<std::string, LuaScript*> m_scripts;
};

#endif