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

			if (resHandle) {
				return 2;
			}
		}

		lua_pushstring(L, "Resource Failed to Loaded");
		return 1;
	}
}

#endif