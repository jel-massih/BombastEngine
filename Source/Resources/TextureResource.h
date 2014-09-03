#ifndef XML_RESOURCE_H
#define XML_RESOURCE_H

#include "ResourceCache.h"

class TextureResourceExtraData : public IResourceExtraData
{
	ID3D11ShaderResourceView* m_pTexture;

public:
	virtual void Shutdown();

	virtual std::string VToString() { return "TextureResourceExtraData"; }
	
	bool LoadTexture(char* pRawBuffer);

	ID3D11ShaderResourceView* GetTexture();
};

class TextureResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return false; }
	virtual unsigned int VGetLoadedResourceSize(char* rawBuffer, unsigned int rawSize) { return rawSize; }
	virtual bool VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle);
	virtual std::string VGetPattern() { return "*.dds"; }

	static ID3D11ShaderResourceView* LoadAndReturnTextureResource(const char* resourceString);
};

#endif