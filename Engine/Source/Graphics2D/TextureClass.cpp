#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_pTexture = NULL;
	m_textureName = "";
}

TextureClass::~TextureClass()
{
	SAFE_RELEASE(m_pTexture);
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