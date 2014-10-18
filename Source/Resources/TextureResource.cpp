#include "TextureResource.h"
#include "../Bombast/BombastApp.h"
#include <fstream>

void TextureResourceExtraData::Shutdown()
{
	SAFE_RELEASE(m_pTexture);
}

ID3D11ShaderResourceView* TextureResourceExtraData::GetTexture()
{
	return m_pTexture;
}

bool TextureResourceExtraData::LoadTexture(char* pRawBuffer, unsigned int rawSize)
{
	HRESULT result;
	
	result = CreateDDSTextureFromMemory(g_pApp->GetGraphicsManager()->GetD3DClass()->GetDevice(), reinterpret_cast<uint8_t*>(pRawBuffer), rawSize, nullptr, &m_pTexture);

	if (FAILED(result)) {
		return false;
	}

	return true;
}

bool TextureResourceLoader::VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle)
{
	bool result;

	if (rawSize <= 0)
	{
		return false;
	}

	TextureResourceExtraData* pExtraData = BE_NEW TextureResourceExtraData();
	result = pExtraData->LoadTexture(rawBuffer, rawSize);

	//Delete buffer since no longer needed after loading texture
	SAFE_DELETE_ARRAY(rawBuffer); 
	
	if (!result) {
		return false;
	}

	handle->SetExtra(pExtraData);

	return true;
}

IResourceLoader* CreateTextureResourceLoader()
{
	return BE_NEW TextureResourceLoader();
}

ID3D11ShaderResourceView* TextureResourceLoader::LoadAndReturnTextureResource(const char* resourceString)
{
	Resource resource(resourceString);
	ResourceHandle* pResourceHandle = g_pApp->m_pResourceCache->GetHandle(&resource);
	TextureResourceExtraData* pExtraData = (TextureResourceExtraData*)(pResourceHandle->GetExtra());

	ID3D11ShaderResourceView* texture = pExtraData->GetTexture();
	return texture;
}