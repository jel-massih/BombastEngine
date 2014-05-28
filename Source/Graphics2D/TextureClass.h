#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "WICTextureLoader.h"
#include "../BombastEngineStd.h"

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass& other);
	~TextureClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, WCHAR* title);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif