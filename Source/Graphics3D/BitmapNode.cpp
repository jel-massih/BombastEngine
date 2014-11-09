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
	m_pBitmap = 0;
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
}

HRESULT D3DBitmapNode11::VOnRestore(Scene* pScene)
{
	HRESULT hr;
	BE_HRETURN(SceneNode::VOnRestore(pScene), "Failed to OnRestore Parent");

	m_bitmapWidth = (int)(g_pApp->m_options.m_screenSize.x * m_relativeSize.x);
	m_bitmapHeight = (int)(g_pApp->m_options.m_screenSize.y * m_relativeSize.y);

	m_pBitmap = BE_NEW BitmapClass();
	if (!m_pBitmap)
	{
		return false;
	}

	m_pBitmap->Initialize(g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice(), m_textureName, m_bitmapWidth, m_bitmapHeight);
	
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
	pRenderer->VEnableZBuffer(false);

	Mat4x4 worldMatrix, viewMatrix, orthoMatrix;
	pRenderer->VGetViewMatrix(viewMatrix);
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetOrthoMatrix(orthoMatrix);

	//prepare bitmap vertex and index buffers for drawing
	result = m_pBitmap->Render(context);
	if (!result)
	{
		return S_FALSE;
	}

	//Render Bitmap with texture shader
	result = g_pApp->GetGraphicsManager()->GetTextureShader()->Render(context, m_pBitmap->GetIndexCount(),
		DirectX::XMLoadFloat4x4(&worldMatrix), DirectX::XMLoadFloat4x4(&viewMatrix), DirectX::XMLoadFloat4x4(&orthoMatrix),
		m_pBitmap->GetTexture());
	if (!result)
	{
		return S_FALSE;
	}
	pRenderer->VEnableZBuffer(true);

	return S_OK;
}

void D3DBitmapNode11::RenderBuffers()
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
	ID3D11DeviceContext* context = pRenderer->GetDeviceContext();

	//Set vertex buffer to actiev in input assembled
	context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//Set index buffer to active
	context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set topology method
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Mat4x4 worldMatrix, viewMatrix, orthoMatrix;
	pRenderer->VGetViewMatrix(viewMatrix);
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetOrthoMatrix(orthoMatrix);

	g_pApp->GetGraphicsManager()->GetTextureShader()->Render(context, GetIndexCount(), DirectX::XMLoadFloat4x4(&worldMatrix), DirectX::XMLoadFloat4x4(&viewMatrix), DirectX::XMLoadFloat4x4(&orthoMatrix), m_pTexture);

	return;
}