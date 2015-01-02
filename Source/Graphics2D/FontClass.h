#pragma once

#include "TextureClass.h"

class FontClass
{
public:
	FontClass();

	bool Initialize(ID3D11Device* device, std::string fontFilename, std::string textureFilename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void* vertices, const char* sentence, float drawX, float drawY);

private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

private:
	bool LoadFontData(const char* filename);
	void ReleaseFontData();
	
	bool LoadTexture(ID3D11Device* device, std::string filename);
	void ReleaseTexture();

private:
	FontType* m_pFont;
	TextureClass* m_pTexture;
};