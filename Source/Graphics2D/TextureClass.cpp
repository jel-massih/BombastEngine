#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_texture = NULL;
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

	result = DirectX::CreateWICTextureFromFile(device, context, textureTitle, NULL, &m_texture, NULL);

	if (FAILED(result)) {
		return false;
	}

	return true;	
}

void TextureClass::Shutdown()
{
	SAFE_RELEASE(m_texture);

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}