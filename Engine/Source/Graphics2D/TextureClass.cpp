#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_pTexture = NULL;
	m_textureName = "";
}

bool TextureClass::Initialize(ID3D11Device* device, std::string textureTitle)
{
	m_textureName = textureTitle;

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