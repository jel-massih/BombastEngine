#ifndef LUA_RESOURCE_H
#define LUA_RESOURCE_H

#include "ResourceCache.h"

class LuaResourceExtraData : public IResourceExtraData
{
public:
	virtual std::string VToString() { return "LuaResourceExtraData"; }

	bool LoadScript(char* pRawBuffer, unsigned int rawSize, std::string filename);
};


//Loads xml files from resource Cache
class LuaResourceLoader : public IResourceLoader
{
public:

	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) { return rawSize; }
	virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, ResourceHandle* handle);
	virtual std::string VGetPattern() { return "*.lua"; }
	virtual bool VAddNullZero() { return true; }

	static void LoadLuaScript(const char* resourceString);
};
#endif