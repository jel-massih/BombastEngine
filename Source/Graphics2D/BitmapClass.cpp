#include "BitmapClass.h"

BitmapClass::BitmapClass()
{
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
	m_pTexture = 0;

	m_posX = 0;
	m_posY = 0;
}

BitmapClass::BitmapClass(const BitmapClass& other)
{
}

BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight)
{
	bool result;

	//Store Screen Size
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Store size in pixels it should be rendered at
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	//Initialize Prev Rendering Pos to -1
	m_previousPosX = -1;
	m_previousPosY = -1;

	//Initialize the Vertex and Index Buffers
	result = InitializeBuffers(device);
	if (!result) {
		return false;
	}

	//Load Texture for Bitmap
	result = LoadTexture(device, textureFilename);
	if (!result) {
		return false;
	}

	return true;
}

void BitmapClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();

	return;
}

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext)
{
	bool result;
	
	result = UpdateBuffers(deviceContext, -(SCREEN_WIDTH/2) + m_posX, -(SCREEN_HEIGHT/2) + m_posY);
	if (!result)
	{
		return false;
	}

	RenderBuffers(deviceContext);

	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_indexCount;
}

void BitmapClass::SetPosition(int x, int y) 
{
	m_posX = x;
	m_posY = y;
}

int BitmapClass::GetBitmapWidth() 
{
	return m_bitmapWidth;
}

int BitmapClass::GetBitmapHeight() 
{
	return m_bitmapHeight;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_pTexture->GetTexture();
}

bool BitmapClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	m_vertexCount = 6;
	m_indexCount = m_vertexCount;

	vertices = BE_NEW VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = BE_NEW unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
	memset(vertices, 0, sizeof(VertexType)* m_vertexCount);
	
	for (i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	//Set up description of static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give Subresource structure pointer to vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Create static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
	
	return true;
}

void BitmapClass::ShutdownBuffers()
{
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	//if position hasnt changed dont update buffers
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	m_previousPosX = positionX;
	m_previousPosY = positionY;

	//calculate screen coords to left side of bitmap
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;
	right = left + (float)m_bitmapWidth;

	top = (float)(m_screenHeight / 2);
	bottom = top - (float)m_bitmapHeight;

	//create vertex array
	vertices = BE_NEW VertexType[m_vertexCount];
	if (!vertices) {
		return false;
	}

	vertices[0].position = DirectX::XMFLOAT3(left, top, 0.0f);  // Bottom left.
	vertices[0].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = DirectX::XMFLOAT3(right, bottom, 0.0f);  // Top middle.
	vertices[1].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = DirectX::XMFLOAT3(left, bottom, 0.0f);  // Bottom right.
	vertices[2].texture = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = DirectX::XMFLOAT3(left, top, 0.0f);  // Bottom left.
	vertices[3].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = DirectX::XMFLOAT3(right, top, 0.0f);  // Top middle.
	vertices[4].texture = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = DirectX::XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

	//Locate vertex buffer for write
	result = deviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	//Get pointer to data in vertex buffer
	verticesPtr = (VertexType*)mappedResource.pData;

	//Copy data into vertex 
	memcpy(verticesPtr, (void*)vertices, sizeof(VertexType)* m_vertexCount);

	//Unlock vertex buffer
	deviceContext->Unmap(m_pVertexBuffer, 0);

	//Release Resources
	SAFE_DELETE_ARRAY(vertices);

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	//Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	
	//Set the index buffer to active so it can be rendered
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set topology method for rendering
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool BitmapClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	m_pTexture = BE_NEW TextureClass;
	if (!m_pTexture)
	{
		return false;
	}

	result = m_pTexture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void BitmapClass::ReleaseTexture()
{
	if (m_pTexture)
	{
		m_pTexture->Shutdown();
		SAFE_DELETE(m_pTexture);
	}

	return;
}