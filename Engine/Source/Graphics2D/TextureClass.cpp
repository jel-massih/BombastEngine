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
	m_pTexture = TextureResourceLoader::LoadAndReturnTextureResource(textureTitle.c_str());
	if (!m_pTexture) {
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