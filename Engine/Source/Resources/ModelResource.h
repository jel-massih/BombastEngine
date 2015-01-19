#pragma once

#include "ResourceCache.h"
#include "../Graphics3D/ModelClass.h"

class ModelClass;

class ModelResourceExtraData : public IResourceExtraData
{
	ModelClass* m_pModelData;

public:
	ModelResourceExtraData() : m_pModelData(nullptr) {}

	virtual void Shutdown();

	virtual std::string VToString() { return "ModelResourceExtraData"; }

	bool LoadModel(char* pRawBuffer, unsigned int rawSize, std::string modelFilepath);

	ModelClass* GetModel();
};

class ModelResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual unsigned int VGetLoadedResourceSize(char* rawBuffer, unsigned int rawSize) { return rawSize; }
	virtual bool VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle);
	virtual std::vector<std::string> VGetPatterns() { return std::vector<std::string>({ "*.bmdl" }); }
	virtual bool VAddNullZero() { return true; }

	static ModelClass* LoadAndReturnModelResource(const char* resourceString);
};