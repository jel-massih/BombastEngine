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

bool TextureClass::Initialize(ID3D11Device* device, std::string textureTitle)
{
	HRESULT result;

	result = CreateDDSTextureFromFile(device, s2ws(textureTitle).c_str(), nullptr, &m_pTexture);

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