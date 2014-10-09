#ifndef LUA_EXPORTS_H
#define LUA_EXPORTS_H

#include <lua.hpp>

namespace InternalLuaExports
{
	int lua_loader(lua_State* L)
	{
		const char* file = lua_tostring(L, 1);
		{
			Resource resource(file);
			ResourceHandle* resHandle = BombastApp::GetGameInstance()->m_pResourceCache->GetHandle(&resource);
		}

		return 1;
	}
}

#endif