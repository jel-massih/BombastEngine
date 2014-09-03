#include "TextureResource.h"
#include "../Bombast/BombastApp.h"

void TextureResourceExtraData::Shutdown()
{
	SAFE_RELEASE(m_pTexture);
}

ID3D11ShaderResourceView* TextureResourceExtraData::GetTexture()
{
	return m_pTexture;
}

bool TextureResourceExtraData::LoadTexture(char* pRawBuffer)
{
	HRESULT result;
	
	result = CreateDDSTextureFromFile(BombastApp::GetGameInstance()->GetGraphicsManager()->GetD3DClass()->GetDevice(), s2ws(pRawBuffer).c_str(), nullptr, &m_pTexture);

	if (FAILED(result)) {
		return false;
	}

	return true;
}

bool TextureResourceLoader::VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle)
{
	if (rawSize <= 0)
	{
		return false;
	}

	TextureResourceExtraData* pExtraData = BE_NEW TextureResourceExtraData();
	pExtraData->LoadTexture(rawBuffer);

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
	ResourceHandle* pResourceHandle = BombastApp::GetGameInstance()->m_pResourceCache->GetHandle(&resource);
	TextureResourceExtraData* pExtraData = (TextureResourceExtraData*)(pResourceHandle->GetExtra());

	ID3D11ShaderResourceView* texture = pExtraData->GetTexture();
	return texture;
}