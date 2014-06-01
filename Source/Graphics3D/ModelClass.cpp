#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
	m_pTexture = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device, WCHAR* textureFilepath)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilepath);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_pTexture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = 3;
	m_indexCount = 3;

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

	vertices[0].position = DirectX::XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f);
	vertices[0].texture = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	vertices[1].position = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	vertices[1].texture = DirectX::XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f);

	vertices[2].position = DirectX::XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f);
	vertices[2].texture = DirectX::XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	if (FAILED(result)) 
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
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

void ModelClass::ShutdownBuffers()
{
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* textureFilepath)
{
	bool result;
	ID3D11DeviceContext* context;

	m_pTexture = BE_NEW TextureClass;
	if (!m_pTexture)
	{
		return false;
	}

	device->GetImmediateContext(&context);

	result = m_pTexture->Initialize(device, context, textureFilepath);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	if (m_pTexture)
	{
		m_pTexture->Shutdown();
		SAFE_DELETE(m_pTexture);
	}
}