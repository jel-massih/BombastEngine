#pragma once

#include "../Resources/TextureResource.h"
#include "../BombastEngineStd.h"

class TextureClass
{
public:
	TextureClass();
	~TextureClass();

	bool Initialize(ID3D11Device* device,  std::string title);

	ID3D11ShaderResourceView* GetTexture() const { return m_pTexture; }
	std::string GetTextureName() const { return m_textureName; }

private:
	ID3D11ShaderResourceView* m_pTexture;

	std::string m_textureName;
};