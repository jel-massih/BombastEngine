#include "BitmapNode.h"
#include "../Bombast/BombastApp.h"

BitmapNode::BitmapNode(const ActorId actorId,
	BaseRenderComponent* renderComponent,
	std::string textureFileName,
	DirectX::XMFLOAT2 relativeSize,
	RenderPass renderPass,
	const Mat4x4 *t) : SceneNode(actorId, renderComponent, renderPass, t)
{
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
	m_pTexture = 0;

	m_relativeSize = relativeSize;

	m_textureName = textureFileName;
}

HRESULT BitmapNode::LoadTexture(std::string filename)
{
	m_pTexture = TextureResourceLoader::LoadAndReturnTextureResource(filename.c_str());
	if (!m_pTexture)
	{
		return S_FALSE;
	}

	return S_OK;
}

D3DBitmapNode11::D3DBitmapNode11(const ActorId actorId,
	BaseRenderComponent* renderComponent,
	std::string textureFileName,
	DirectX::XMFLOAT2 relativeSize,
	RenderPass renderPass,
	const Mat4x4* t) : BitmapNode(actorId, renderComponent, textureFileName, relativeSize, renderPass, t)
{}

HRESULT D3DBitmapNode11::VOnRestore(Scene* pScene)
{
	HRESULT hr;
	BE_HRETURN(SceneNode::VOnRestore(pScene), "Failed to OnRestore Parent");

	//Reload the Texture
	{
		Resource resource(m_textureName);
		ResourceHandle* pResHandle = g_pApp->m_pResourceCache->GetHandle(&resource);
		TextureResourceExtraData* extra = static_cast<TextureResourceExtraData*>(pResHandle->GetExtra());
	}

	m_bitmapWidth = (int)(g_pApp->m_options.m_screenSize.x * m_relativeSize.x);
	m_bitmapWidth = (int)(g_pApp->m_options.m_screenSize.y * m_relativeSize.y);

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

HRESULT D3DBitmapNode11::InitializeBuffers()
{
	ID3D11Device* device = g_pApp->GetGraphicsManager()->GetD3DClass()->GetDevice();

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

	//Get Texture
	Resource resource(m_textureName);
	ResourceHandle* pResourceHandle = g_pApp->m_pResourceCache->GetHandle(&resource);
	TextureResourceExtraData* extra = static_cast<TextureResourceExtraData*>(pResourceHandle->GetExtra());

	//Render

	return S_OK;
}