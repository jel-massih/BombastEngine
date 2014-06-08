#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "../Utilities/DDSTextureLoader.h"
#include "../BombastEngineStd.h"

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass& other);
	~TextureClass();

	bool Initialize(ID3D11Device* device,  WCHAR* title);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_pTexture;
};

#endif