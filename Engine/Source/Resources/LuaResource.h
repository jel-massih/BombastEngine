#pragma once

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
	virtual std::vector<std::string> VGetPatterns() { return std::vector<std::string>({ "*.lua" }); }
	virtual bool VAddNullZero() { return true; }

	static void LoadLuaScript(const char* resourceString);
};