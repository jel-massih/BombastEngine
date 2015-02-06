#include "TextureResource.h"
#include "../Bombast/BombastApp.h"
#include <fstream>

const char* TextureResourceLoader::m_defaultTextureName= "textures\\grid.dds";

void TextureResourceExtraData::Shutdown()
{
	SAFE_RELEASE(m_pTexture);
}

ID3D11ShaderResourceView* TextureResourceExtraData::GetTexture()
{
	return m_pTexture;
}

bool TextureResourceExtraData::LoadTexture(char* pRawBuffer, unsigned int rawSize, std::string extension)
{
	HRESULT result;
	
	if (extension == ".dds")
	{
		result = CreateDDSTextureFromMemory(g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice(), reinterpret_cast<uint8_t*>(pRawBuffer), rawSize, nullptr, &m_pTexture);
	}
	else
	{
		result = CreateWICTextureFromMemory(g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice(), reinterpret_cast<uint8_t*>(pRawBuffer), rawSize, nullptr, &m_pTexture);
	}

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

	
	std::string fileExtension = handle->GetName().substr(handle->GetName().find_last_of("."));

	result = pExtraData->LoadTexture(rawBuffer, rawSize, fileExtension);

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
	if (!pResourceHandle)
	{
		BE_ERROR("Texture not found, loading default texture: %s", m_defaultTextureName);
		return LoadDefaultTextureResource();
	}
	TextureResourceExtraData* pExtraData = (TextureResourceExtraData*)(pResourceHandle->GetExtra());

	ID3D11ShaderResourceView* texture = pExtraData->GetTexture();
	return texture;
}

ID3D11ShaderResourceView* TextureResourceLoader::LoadDefaultTextureResource()
{
	Resource resource(m_defaultTextureName);
	ResourceHandle* pResourceHandle = g_pApp->m_pResourceCache->GetHandle(&resource);
	TextureResourceExtraData* pExtraData = (TextureResourceExtraData*)(pResourceHandle->GetExtra());

	ID3D11ShaderResourceView* texture = pExtraData->GetTexture();
	return texture;
}