#ifndef LUA_RESOURCE_H
#define LUA_RESOURCE_H

#include "ResourceCache.h"
#include "../Scripting/LuaScript.h"

class LuaResourceExtraData : public IResourceExtraData
{
	LuaScript* m_pLuaScript;

public:
	LuaResourceExtraData();

	virtual void Shutdown();

	virtual std::string VToString() { return "LuaResourceExtraData"; }

	bool LoadScript(char* pRawBuffer, unsigned int rawSize, std::string filename);

	LuaScript* GetScript();
};


//Loads xml files from resource Cache
class LuaResourceLoader : public IResourceLoader
{
public:

	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return false; }
	virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) { return rawSize; }
	virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, ResourceHandle* handle);
	virtual std::string VGetPattern() { return "*.lua"; }
	virtual bool VAddNullZero() { return true; }

	static LuaScript* LoadAndReturnLuaScript(const char* resourceString);
};
#endif