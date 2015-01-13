#pragma once

class FontClass;
class FontShader;

class DebugText
{
public:
	DebugText();

	bool Initialize();
	void Shutdown();
	bool Render();

	bool AddString(const char* stringId);
	bool AddString(char* stringId, char* text, int posX, int posY);
	bool AddString(char* stringId, char* text, int posX, int posY, float r, float g, float b);

	bool UpdateString(const char* stringId, const char* text);

private:
	struct SentenceType
	{
		const char* stringId;
		ID3D11Buffer* vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

private:
	bool InitializeSentence(SentenceType** sentence, const char* stringId, int maxLength, ID3D11Device* device);
	bool UpdateSentence(SentenceType* sentence,  const char* text, int posX, int posY, float r, float g, float b, ID3D11DeviceContext* deviceContext);
	void ReleaseSentence(SentenceType** sentence);
	bool RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, Mat4x4& worldMatrix, Mat4x4& orthoMatrix);

private:
	FontClass* m_pFont;
	FontShader* m_pFontShader;
	int m_screenWidth, m_screenHeight;
	Mat4x4 m_baseViewMatrix;
	SentenceType** m_pStrings;
	int m_stringCount;
};