#pragma once

#include "ResourceCache.h"

class TextureResourceExtraData : public IResourceExtraData
{
	ID3D11ShaderResourceView* m_pTexture;

public:
	virtual void Shutdown();

	virtual std::string VToString() { return "TextureResourceExtraData"; }
	
	bool LoadTexture(char* pRawBuffer, unsigned int rawSize, std::string extension);

	ID3D11ShaderResourceView* GetTexture();
};

class TextureResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return false; }
	virtual unsigned int VGetLoadedResourceSize(char* rawBuffer, unsigned int rawSize) { return rawSize; }
	virtual bool VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle);
	virtual std::vector<std::string> VGetPatterns() { return std::vector<std::string>({ "*.dds", "*.jpg", "*.png" }); }

	static ID3D11ShaderResourceView* LoadAndReturnTextureResource(const char* resourceString);
};