#pragma once

class MaterialResourceExtraData : public IResourceExtraData
{
	Material* m_pMaterial;

public:
	MaterialResourceExtraData() : m_pMaterial(nullptr) {}

	virtual void Shutdown();

	virtual std::string VToString() { return "MaterialResourceExtraData"; }

	bool LoadMaterial(char* pRawBuffer, unsigned int rawSize);

	Material* GetMaterial();
};

class MaterialResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return false; }
	virtual unsigned int VGetLoadedResourceSize(char* rawBuffer, unsigned int rawSize) { return rawSize; }
	virtual bool VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle);
	virtual std::string VGetPattern() { return "*.bmtl"; }
	virtual bool VAddNullZero() { return true; }

	static Material* LoadAndReturnMaterialResource(const char* resourceString);
};