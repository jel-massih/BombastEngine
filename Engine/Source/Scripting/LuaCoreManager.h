#pragma once

#include <map>
#include <lua.hpp>

class LuaCoreManager {
public:
	LuaCoreManager():L(nullptr){}
	~LuaCoreManager();

	bool Initialize();

	bool LoadScript(std::string fileName);
	bool LoadScriptFromBuffer(char* rawBuffer, int rawSize, std::string filename);

	void PrintError(const std::string& variableName, const std::string& reason);

	inline void clean()
	{
		int n = lua_gettop(L);
		lua_pop(L, n);
	}

	template<typename T>
	T get(const std::string& variableName)
	{
		if (!L)
		{
			PrintError(variableName, "Script is not loaded");
			return lua_getdefault<T>();
		}

		T result;
		if (lua_gettostack(variableName))
		{
			result = lua_get<T>(variableName);
		}
		else
		{
			result = lua_getdefault<T>();
		}

		clean();
		return result;
	}

	bool lua_gettostack(const std::string& variableName)
	{
		m_level = 0;

		std::string var = "";
		for (unsigned int i = 0; i<variableName.size(); i++)
		{
			if (variableName.at(i) == '.') {
				if (m_level == 0)
				{
					lua_getglobal(L, var.c_str());
				}
				else
				{
					lua_getfield(L, -1, var.c_str());
				}

				if (lua_isnil(L, -1))
				{
					PrintError(variableName, var + " is not defined");
					return false;
				}
				else {
					var = "";
					m_level++;
				}
			}
			else
			{
				var += variableName.at(i);
			}
		}

		if (m_level == 0)
		{
			lua_getglobal(L, var.c_str());
		}
		else
		{
			lua_getfield(L, -1, var.c_str());
		}

		if (lua_isnil(L, -1))
		{
			PrintError(variableName, var + " is Not defined");
			return false;
		}

		return true;
	}


	template<typename T>
	T lua_get(const std::string& variableName) {
		return 0;
	}

	template<typename T>
	T lua_getdefault() {
		return 0;
	}

private:
	bool RegisterLoader();

	lua_State* L;
	int m_level;
};

template <>
inline bool LuaCoreManager::lua_get<bool>(const std::string& variableName) {
	return lua_toboolean(L, -1) != 0;
}

template <>
inline float LuaCoreManager::lua_get<float>(const std::string& variableName) {
	if (!lua_isnumber(L, -1)) {
		PrintError(variableName, "Not a number");
	}
	return (float)lua_tonumber(L, -1);
}

template <>
inline int LuaCoreManager::lua_get<int>(const std::string& variableName) {
	if (!lua_isnumber(L, -1)) {
		PrintError(variableName, "Not a number");
	}
	return (int)lua_tonumber(L, -1);
}

template <>
inline std::string LuaCoreManager::lua_get<std::string>(const std::string& variableName) {
	std::string s = "null";
	if (lua_isstring(L, -1)) {
		s = std::string(lua_tostring(L, -1));
	}
	else {
		PrintError(variableName, "Not a string");
	}
	return s;
}

template<>
inline std::string LuaCoreManager::lua_getdefault<std::string>() {
	return "null";
}