#pragma once

class TextureClass;

struct FontCharacter
{
	float startU;
	float endU;
	float startV;
	float endV;
	int size;
};

class Font
{
public:
	Font();
	~Font();

	bool Initialize(ID3D11Device* device, std::string fontFilename, std::string textureFilename);

	ID3D11ShaderResourceView* GetTexture();

	FontCharacter GetCharacter(TCHAR character) const;

	static const float FontKerning;

private:
	bool LoadFontData(const char* filename);

	bool LoadTexture(ID3D11Device* device, std::string filename);

private:
	FontCharacter* m_pFontCharacters;
	TextureClass* m_pTexture;
};