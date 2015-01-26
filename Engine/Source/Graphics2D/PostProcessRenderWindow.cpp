#include "PostProcessRenderWindow.h"

PostProcessRenderWindow::~PostProcessRenderWindow()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

bool PostProcessRenderWindow::Initialize(ID3D11Device* device, int width, int height)
{
	if (!InitializeBuffers(device, width, height))
	{
		return false;
	}

	return true;
}

void PostProcessRenderWindow::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

bool PostProcessRenderWindow::InitializeBuffers(ID3D11Device* device, int width, int height)
{
	float left, right, top, bottom;
	VertexType* verts;
	unsigned long* indices;
	HRESULT result;

	left = (float)((width / 2) * -1);
	right = left + (float)width;

	top = (float)(height / 2);
	bottom = top - (float)height;

	m_vertexCount = 6;
	m_indexCount = m_vertexCount;

	verts = new VertexType[m_vertexCount];
	if (!verts)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	verts[0].pos = XMFLOAT3(left, top, 0.0f);  // Top left.
	verts[0].uv = XMFLOAT2(0.0f, 0.0f);

	verts[1].pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	verts[1].uv = XMFLOAT2(1.0f, 1.0f);

	verts[2].pos = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	verts[2].uv = XMFLOAT2(0.0f, 1.0f);


	verts[3].pos = XMFLOAT3(left, top, 0.0f);  // Top left.
	verts[3].uv = XMFLOAT2(0.0f, 0.0f);

	verts[4].pos = XMFLOAT3(right, top, 0.0f);  // Top right.
	verts[4].uv = XMFLOAT2(1.0f, 0.0f);

	verts[5].pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	verts[5].uv = XMFLOAT2(1.0f, 1.0f);

	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	//Vertex Buffer
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VertexType) * m_vertexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = verts;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		result = device->CreateBuffer(&bd, &vertexData, &m_pVertexBuffer);
		if (FAILED(result))
		{
			return false;
		}
	}

	//Index Buffer
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned long) * m_indexCount;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = indices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		result = device->CreateBuffer(&bd, &data, &m_pIndexBuffer);
		if (FAILED(result))
		{
			return false;
		}
	}

	SAFE_DELETE_ARRAY(verts);
	SAFE_DELETE_ARRAY(indices);

	return true;
}

void PostProcessRenderWindow::RenderBuffers(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}