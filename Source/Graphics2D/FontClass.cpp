#include "FontClass.h"

FontClass::FontClass()
{

}

bool FontClass::Initialize(ID3D11Device* device, std::string fontFilename, std::string textureFilename)
{
	if (!LoadFontData(fontFilename.c_str()))
	{
		return false;
	}

	if (!LoadTexture(device, textureFilename.c_str()))
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	ReleaseTexture();

	ReleaseFontData();
}

bool FontClass::LoadFontData(const char* filename)
{
	std::ifstream fin;
	int i;
	char temp;

	m_font = new FontType[95];
	if (!m_font)
	{
		return false;
	}

	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}

	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	if (m_font)
	{
		delete[] m_font;
		m_font = 0;
	}
}

bool FontClass::LoadTexture(ID3D11Device* device, std::string filename)
{
	m_texture = new TextureClass;
	if (!m_texture)
	{
		return false;
	}

	if (!m_texture->Initialize(device, filename))
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, const char* text, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	vertexPtr = static_cast<VertexType*> (vertices);

	numLetters = static_cast<int>(strlen(text));

	index = 0;

	for (i = 0; i < numLetters; i++)
	{
		letter = static_cast<int>(text[i]) - 32;

		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_font[letter].size, drawY - 16, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, drawY - 16, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_font[letter].size, drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_font[letter].size, drawY - 16, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 1.0f);
			index++;

			drawX = drawX + m_font[letter].size + 1.0f;
		}
	}
}