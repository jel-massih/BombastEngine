#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_pTexture = NULL;
}

TextureClass::TextureClass(const TextureClass& other)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, wchar_t* textureTitle)
{
	HRESULT result;

	result = CreateDDSTextureFromFile(device, textureTitle, nullptr, &m_pTexture);

	if (FAILED(result)) {
		return false;
	}

	return true;	
}

void TextureClass::Shutdown()
{
	SAFE_RELEASE(m_pTexture);

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_pTexture;
}