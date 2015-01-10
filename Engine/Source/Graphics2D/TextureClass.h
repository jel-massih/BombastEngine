#pragma once

#include "../Resources/TextureResource.h"
#include "../BombastEngineStd.h"

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass& other);
	~TextureClass();

	bool Initialize(ID3D11Device* device,  std::string title);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_pTexture;
};