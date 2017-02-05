#include "DebugText.h"

#include "../Bombast/BombastApp.h"
#include "../Rendering/Font.h"
#include "../Shading/FontShader.h"

DebugText::DebugText()
{
	m_pFont = 0;
	m_pFontShader = 0;

	m_pStrings = 0;
}

DebugText::~DebugText()
{
	if (m_pStrings)
	{
		for (int i = 0; i < m_stringCount; i++)
		{
			ReleaseSentence(&m_pStrings[i]);
		}

		SAFE_DELETE(m_pStrings);
	}

	SAFE_DELETE(m_pFontShader);

	SAFE_DELETE(m_pFont);
}

bool DebugText::Initialize()
{
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	bool result;

	device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();
	deviceContext = g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext();

	Point screenSize = g_pApp->GetScreenSize();

	m_screenWidth = screenSize.GetX();
	m_screenHeight = screenSize.GetY();

	g_pApp->GetGraphicsManager()->GetRenderer()->VGetViewMatrix(m_baseViewMatrix);

	m_stringCount = 0;

	m_pFont = BE_NEW Font;
	if (!m_pFont)
	{
		return false;
	}
	
	result = m_pFont->Initialize(device, "EngineResources.Data.fontdata.txt", "EngineResources.Textures.font.dds");
	if (!result)
	{
		BE_ERROR("Could not initialize the font object");
		return false;
	}

	m_pFontShader = BE_NEW FontShader;
	if (!m_pFontShader)
	{
		return false;
	}

	result = m_pFontShader->Initialize(device);
	if (!result)
	{
		BE_ERROR("Could not initialize the Font Shader Object");
		return false;
	}

	return true;
}

bool DebugText::Render()
{
	bool result;
	ID3D11DeviceContext* deviceContext = g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext();

	Mat4x4 worldMatrix, orthoMatrix;

	g_pApp->GetGraphicsManager()->GetRenderer()->VGetWorldMatrix(worldMatrix);
	g_pApp->GetGraphicsManager()->GetRenderer()->VGetOrthoMatrix(orthoMatrix);

	for (int i = 0; i < m_stringCount; i++)
	{
		result = RenderSentence(deviceContext, m_pStrings[i], worldMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}
	}

	return true;
}

bool DebugText::InitializeSentence(SentenceType** sentence, const char* stringId, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	*sentence = BE_NEW SentenceType;
	if (!*sentence)
	{
		return false;
	}

	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	(*sentence)->maxLength = maxLength;
	(*sentence)->vertexCount = 6 * maxLength;
	(*sentence)->indexCount = (*sentence)->vertexCount;
	(*sentence)->stringId = stringId;

	vertices = BE_NEW VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = BE_NEW unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	memset(vertices, 0, sizeof(VertexType) * (*sentence)->vertexCount);

	for (i = 0; i < (*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);

	if (FAILED(result))
	{
		return false;
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	return true;
}

bool DebugText::UpdateSentence(SentenceType* sentence, const char* text, int posX, int posY, float r, float g, float b, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	sentence->red = r;
	sentence->green = g;
	sentence->blue = b;

	numLetters = (int)strlen(text);

	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	vertices = BE_NEW VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	memset(vertices, 0, sizeof(VertexType) * sentence->vertexCount);

	drawX = (float)(((m_screenWidth / 2) * -1) + posX);
	drawY = (float)((m_screenHeight / 2) - posY);

	m_pFont->BuildVertexArray((void*)vertices, text, drawX, drawY);

	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, sizeof(VertexType) * sentence->vertexCount);

	deviceContext->Unmap(sentence->vertexBuffer, 0);

	SAFE_DELETE_ARRAY(vertices);

	return true;
}

void DebugText::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		SAFE_RELEASE((*sentence)->vertexBuffer);
		SAFE_RELEASE((*sentence)->indexBuffer);

		SAFE_DELETE(*sentence);
	}
}

bool DebugText::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, Mat4x4& worldMatrix, Mat4x4& orthoMatrix)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	result = m_pFontShader->Render(deviceContext, sentence->indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&m_baseViewMatrix), XMLoadFloat4x4(&orthoMatrix), m_pFont->GetTexture(), pixelColor);
	if (!result)
	{
		return false;
	}

	return true;
}

bool DebugText::AddString(const char* stringId)
{
	bool result;
	SentenceType** tempStrings;

	ID3D11Device* device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();

	tempStrings = m_pStrings;

	m_pStrings = BE_NEW SentenceType*[m_stringCount + 1];
	if (!m_pStrings)
	{
		return false;
	}

	for (int i = 0; i < m_stringCount; i++)
	{
		m_pStrings[i] = tempStrings[i];
	}

	result = InitializeSentence(&m_pStrings[m_stringCount], stringId, 32, device);
	if (!result)
	{
		return false;
	}

	m_stringCount++;

	SAFE_DELETE(tempStrings);

	return true;
}

bool DebugText::AddString(char* stringId, char* text, int posX, int posY)
{
	return false;
}

bool DebugText::AddString(char* stringId, char* text, int posX, int posY, float r, float g, float b)
{
	return false;
}

bool DebugText::UpdateString(const char* stringId, const char* text)

{
	bool result;
	int index;

	ID3D11DeviceContext* deviceContext = g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext();

	for (index = 0; index < m_stringCount; index++)
	{
		if (m_pStrings[index]->stringId == stringId)
		{
			result = UpdateSentence(m_pStrings[index], text, 20, 10 + index * 20, 1.0f, 1.0f, 1.0f, deviceContext);
			if (!result)
			{
				return false;
			}
		}
	}

	return true;
}