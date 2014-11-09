#include "BitmapNode.h"
#include "../Bombast/BombastApp.h"

BitmapNode::BitmapNode(const ActorId actorId,
	BaseRenderComponent* renderComponent,
	std::string textureFileName,
	DirectX::XMFLOAT2 relativeSize,
	RenderPass renderPass,
	const Mat4x4 *t) : SceneNode(actorId, renderComponent, renderPass, t)
{
	m_relativeSize = relativeSize;
	m_textureName = textureFileName;
}

bool BitmapNode::VIsVisible(Scene* pScene) const
{
	return true;
}

D3DBitmapNode11::D3DBitmapNode11(const ActorId actorId,
	BaseRenderComponent* renderComponent,
	std::string textureFileName,
	DirectX::XMFLOAT2 relativeSize,
	RenderPass renderPass,
	const Mat4x4* t) : BitmapNode(actorId, renderComponent, textureFileName, relativeSize, renderPass, t)
{
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
	m_pTexture = 0;

	m_lastPosition = Vec3::g_InvalidVec3;
}

HRESULT D3DBitmapNode11::VOnRestore(Scene* pScene)
{
	HRESULT hr;
	BE_HRETURN(SceneNode::VOnRestore(pScene), "Failed to OnRestore Parent");

	m_bitmapWidth = (int)(g_pApp->m_options.m_screenSize.x * m_relativeSize.x);
	m_bitmapHeight = (int)(g_pApp->m_options.m_screenSize.y * m_relativeSize.y);

	hr = InitializeBuffers();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = LoadTexture(m_textureName);
	if (FAILED(hr))
	{
		return hr;
	}
	
	return S_OK;
}

HRESULT D3DBitmapNode11::LoadTexture(std::string filename)
{
	m_pTexture = TextureResourceLoader::LoadAndReturnTextureResource(filename.c_str());
	if (!m_pTexture)
	{
		return S_FALSE;
	}

	return S_OK;
}

HRESULT D3DBitmapNode11::InitializeBuffers()
{
	ID3D11Device* device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();

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
		return S_FALSE;
	}

	indices = BE_NEW unsigned long[m_indexCount];
	if (!indices)
	{
		return S_FALSE;
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
		return result;
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
		return result;
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	return S_OK;
}

HRESULT D3DBitmapNode11::VRender(Scene* pScene)
{
	HRESULT hr;
	bool result;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
	ID3D11DeviceContext* context = pRenderer->GetDeviceContext();

	Mat4x4 worldMatrix, viewMatrix, orthoMatrix;
	pRenderer->VGetViewMatrix(viewMatrix);
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetOrthoMatrix(orthoMatrix);

	//prepare bitmap vertex and index buffers for drawing
	hr = UpdateBuffers(context);
	if(FAILED(hr))
	{
		return hr;
	}

	RenderBuffers(context);

	//Render Bitmap with texture shader
	result = g_pApp->GetGraphicsManager()->GetTextureShader()->Render(context, m_indexCount,
		DirectX::XMLoadFloat4x4(&worldMatrix), DirectX::XMLoadFloat4x4(&viewMatrix), DirectX::XMLoadFloat4x4(&orthoMatrix),
		m_pTexture);
	if (!result)
	{
		return S_FALSE;
	}

	return S_OK;
}

HRESULT D3DBitmapNode11::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;
	
	//if position hasnt changed dont update buffers
	if (VGet()->ToWorld().GetPosition() == m_lastPosition)
	{
		return true;
	}
	m_lastPosition = VGet()->ToWorld().GetPosition();

	left = (float)(g_pApp->m_options.m_screenSize.x / 2 * -1) + m_lastPosition.x;
	right = left + (float)m_bitmapWidth;

	top = (float)(g_pApp->m_options.m_screenSize.y / 2) - m_lastPosition.y;
	bottom = top - (float)m_bitmapHeight;

	//create vertex array
	vertices = BE_NEW VertexType[m_vertexCount];
	if (!vertices) {
		return false;
	}
	//_RPT1(0, "Left: %d\n", positionX);

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

void D3DBitmapNode11::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();

	//Set vertex buffer to actiev in input assembled
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//Set index buffer to active
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set topology method
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}