#pragma once

#include "ResourceCache.h"

class ShaderResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return true; }
	virtual bool VDiscardRawBufferAfterLoad() { return false; }
	virtual unsigned int VGetLoadedResourceSize(char* rawBuffer, unsigned int rawSize) { return rawSize; }
	virtual bool VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle) { return true; }
	virtual std::string VGetPattern() { return "*.cso"; }
};