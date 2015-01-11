#include "RenderNodes.h"
#include "ModelClass.h"
#include "../Resources/ModelResource.h"

BitmapNode::BitmapNode(const ActorId actorId,
	BaseRenderComponent* renderComponent,
	std::string textureFileName,
	DirectX::XMFLOAT2 relativeSize,
	RenderPass renderPass,
	const Mat4x4 *t) : SceneNode(actorId, renderComponent, renderPass, t)
	, m_textureName(textureFileName), m_relativeSize(relativeSize)
{
}

D3DBitmapNode11::D3DBitmapNode11(const ActorId actorId,
	BaseRenderComponent* renderComponent,
	std::string textureFileName,
	DirectX::XMFLOAT2 relativeSize,
	RenderPass renderPass,
	const Mat4x4* t) : BitmapNode(actorId, renderComponent, textureFileName, relativeSize, renderPass, t)
	, m_lastPosition(Vec3::g_InvalidVec3)
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
	if (FAILED(hr))
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

	left = -m_lastPosition.x;
	right = left + (float)m_bitmapWidth;

	top = -m_lastPosition.y;
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

D3D11GridNode::D3D11GridNode(ActorId actorId, BaseRenderComponent* renderComponent, const Mat4x4* pMatrix)
	: SceneNode(actorId, renderComponent, RenderPass_0, pMatrix), m_bShow(true)
{
	m_vertCount = m_indexCount = 0;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;

	m_gridWidth = 100;
	m_gridHeight = 100;

	SetPosition(Vec3(-(m_gridWidth / 2), 0, -(m_gridHeight / 2)));
}

D3D11GridNode::~D3D11GridNode()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

HRESULT D3D11GridNode::VOnRestore(Scene* pScene)
{
	HRESULT hr;

	BE_HRETURN(SceneNode::VOnRestore(pScene), "Failed to restore Scenenode for Grid");

	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

	if (!InitializeBuffers())
	{
		return S_FALSE;
	}

	return S_OK;
}

HRESULT D3D11GridNode::InitializeBuffers()
{
	ID3D11Device* device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();

	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	float posX, posZ;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertCount = (m_gridWidth)* (m_gridHeight)* 8;
	m_indexCount = m_vertCount;

	vertices = new VertexType[m_vertCount];
	if (!vertices)
	{
		return S_FALSE;
	}

	indices = BE_NEW unsigned long[m_indexCount];
	if (!indices)
	{
		return S_FALSE;
	}

	index = 0;
	for (i = 0; i < m_gridHeight; i++)
	{
		for (j = 0; j < m_gridWidth; j++)
		{
			//Line 1
			posX = (float)j;
			posZ = (float)(i + 1);

			vertices[index].position = Vec3(posX, 0.0f, posZ);
			vertices[index].color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			posX = (float)(j + 1);
			posZ = (float)(i + 1);
			vertices[index].position = Vec3(posX, 0.0f, posZ);
			vertices[index].color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			//Line 2
			posX = (float)(j + 1);
			posZ = (float)(i + 1);
			vertices[index].position = Vec3(posX, 0.0f, posZ);
			vertices[index].color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			posX = (float)(j + 1);
			posZ = (float)i;
			vertices[index].position = Vec3(posX, 0.0f, posZ);
			vertices[index].color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			//Line 3
			posX = (float)(j + 1);
			posZ = (float)i;
			vertices[index].position = Vec3(posX, 0.0f, posZ);
			vertices[index].color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			posX = (float)j;
			posZ = (float)i;
			vertices[index].position = Vec3(posX, 0.0f, posZ);
			vertices[index].color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			//Line 4
			posX = (float)j;
			posZ = (float)i;
			vertices[index].position = Vec3(posX, 0.0f, posZ);
			vertices[index].color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			posX = (float)j;
			posZ = (float)(i + 1);
			vertices[index].position = Vec3(posX, 0.0f, posZ);
			vertices[index].color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;
		}
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertCount;
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
		return result;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(long) * m_indexCount;
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

HRESULT D3D11GridNode::VRender(Scene* pScene)
{
	bool result;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
	ID3D11DeviceContext* context = pRenderer->GetDeviceContext();

	Mat4x4 worldMatrix, viewMatrix, projectionMatrix;
	pRenderer->VGetViewMatrix(viewMatrix);
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetProjectionMatrix(projectionMatrix);

	RenderBuffers(context);

	result = g_pApp->GetGraphicsManager()->GetColorShader()->Render(context, m_indexCount, DirectX::XMLoadFloat4x4(&worldMatrix),
		XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix));
	if (!result)
	{
		return S_FALSE;
	}

	return S_OK;
}

void D3D11GridNode::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

PrimitiveNode::PrimitiveNode(const ActorId actorId, BaseRenderComponent* renderComponent, std::string textureFilename, RenderPass renderPass, PrimitiveType type, Vec3 size, const Mat4x4* t)
	: SceneNode(actorId, renderComponent, renderPass, t)
{
	m_vertCount = m_indexCount = 0;
	m_size = size;
	m_textureFilename = textureFilename;
	m_primitiveType = type;
}

bool PrimitiveNode::VIsVisible(Scene* pScene) const
{
	return true;
}

D3D11PrimitiveNode::D3D11PrimitiveNode(const ActorId actorId, BaseRenderComponent* renderComponent, std::string textureFilename, RenderPass renderPass, PrimitiveType type, Vec3 size, const Mat4x4* t)
	:PrimitiveNode(actorId, renderComponent, textureFilename, renderPass, type, size, t)
{
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pTexture = NULL;

	m_lastPos = Vec3::g_InvalidVec3;
}

D3D11PrimitiveNode::~D3D11PrimitiveNode()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pTexture);
}

HRESULT D3D11PrimitiveNode::VOnRestore(Scene* pScene)
{
	HRESULT hr;

	BE_HRETURN(SceneNode::VOnRestore(pScene), "Failed to restore scenenode for primitiveNode");

	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

	hr = InitializeBuffers();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = LoadTexture(m_textureFilename);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT D3D11PrimitiveNode::LoadTexture(std::string textureFilename)
{
	m_pTexture = TextureResourceLoader::LoadAndReturnTextureResource(textureFilename.c_str());
	if (!m_pTexture)
	{
		return S_FALSE;
	}

	return S_OK;
}

HRESULT D3D11PrimitiveNode::InitializeBuffers()
{
	ID3D11Device* device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	if (m_primitiveType == PrimitiveType::PT_Box)
	{
		float radius = 1.0f;

		m_vertCount = 20;
		m_indexCount = 36;

		VertexType vertices[] = {
			{ Vec3(1, -1, -1), XMFLOAT2(0, 0) },
			{ Vec3(1, -1, 1), XMFLOAT2(1, 0) },
			{ Vec3(-1, -1, 1), XMFLOAT2(1, 1) },
			{ Vec3(-1, -1, -1), XMFLOAT2(0, 1) },
			{ Vec3(1, 1, -1), XMFLOAT2(1, 0) },
			{ Vec3(1, 1, 1), XMFLOAT2(0, 1) },
			{ Vec3(-1, 1, 1), XMFLOAT2(1, 1) },
			{ Vec3(-1, 1, -1), XMFLOAT2(1, 0) },

			{ Vec3(1, 1, 1), XMFLOAT2(1, 1) },
			{ Vec3(1, -1, 1), XMFLOAT2(0, 0) },
			{ Vec3(1, 1, 1), XMFLOAT2(1, 0) },
			{ Vec3(-1, 1, 1), XMFLOAT2(1, 0) },

			{ Vec3(-1, 1, -1), XMFLOAT2(1, 1) },
			{ Vec3(1, 1, -1), XMFLOAT2(0, 0) },
			{ Vec3(1, -1, 1), XMFLOAT2(0, 1) },
			{ Vec3(-1, -1, 1), XMFLOAT2(0, 1) },

			{ Vec3(-1, -1, 1), XMFLOAT2(0, 0) },
			{ Vec3(1, -1, -1), XMFLOAT2(1, 0) },
			{ Vec3(-1, 1, -1), XMFLOAT2(0, 1) },
			{ Vec3(-1, -1, -1), XMFLOAT2(1, 1) },
		};

		WORD indices[] = {
			1, 2, 3,
			7, 6, 5,
			0, 4, 8,
			9, 10, 6,
			11, 12, 3,
			17, 19, 18,
			0, 1, 3,
			13, 7, 5,
			14, 0, 8,
			15, 9, 6,
			16, 11, 3,
			13, 17, 18,
		};

		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;

		ZeroMemory(&vertexData, sizeof(vertexData));
		vertexData.pSysMem = vertices;

		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
		if (FAILED(result))
		{
			return result;
		}

		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(WORD) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;

		ZeroMemory(&indexData, sizeof(indexData));
		indexData.pSysMem = indices;

		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
		if (FAILED(result))
		{
			return result;
		}
	}

	return S_OK;
}

HRESULT D3D11PrimitiveNode::VRender(Scene* pScene)
{
	bool result;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
	ID3D11DeviceContext* context = pRenderer->GetDeviceContext();

	Mat4x4 worldMatrix, viewMatrix, projectionMatrix;
	pRenderer->VGetViewMatrix(viewMatrix);
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetProjectionMatrix(projectionMatrix);

	RenderBuffers(context);

	result = g_pApp->GetGraphicsManager()->GetTextureShader()->Render(context, m_indexCount, DirectX::XMLoadFloat4x4(&worldMatrix),
		XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), m_pTexture);
	if (!result)
	{
		return S_FALSE;
	}

	return S_OK;
}

void D3D11PrimitiveNode::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

MeshNode::MeshNode(const ActorId actorId, BaseRenderComponent* renderComponent, std::string textureFileName, std::string meshFileName, RenderPass renderPass, const Mat4x4 *t)
	: SceneNode(actorId, renderComponent, renderPass, t), m_meshFilename(meshFileName), m_textureFilename(textureFileName)
{
	m_vertexCount = m_indexCount = 0;
}

D3DMeshNode11::D3DMeshNode11(const ActorId actorId, BaseRenderComponent* renderComponent, std::string textureFilename, std::string meshFilename, RenderPass renderPass, const Mat4x4* t)
	: MeshNode(actorId, renderComponent, textureFilename, meshFilename, renderPass, t), m_lastPosition(Vec3::g_InvalidVec3)
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTexture = nullptr;
	m_pLoadedMesh = nullptr;
}

D3DMeshNode11::~D3DMeshNode11()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pTexture);
}

HRESULT D3DMeshNode11::VOnRestore(Scene* pScene)
{
	HRESULT hr;

	BE_HRETURN(SceneNode::VOnRestore(pScene), "Failed to restore scenenode for D3DMeshNode11");

	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

	hr = LoadMesh(m_meshFilename);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = InitializeBuffers();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = LoadTexture(m_textureFilename);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT D3DMeshNode11::LoadTexture(std::string textureFilename)
{
	m_pTexture = TextureResourceLoader::LoadAndReturnTextureResource(textureFilename.c_str());
	if (!m_pTexture)
	{
		return S_FALSE;
	}

	return S_OK;
}

HRESULT D3DMeshNode11::LoadMesh(std::string meshFilename)
{
	m_pLoadedMesh = ModelResourceLoader::LoadAndReturnModelResource(meshFilename.c_str());;
	if (!m_pLoadedMesh)
	{
		return S_FALSE;
	}

	return S_OK;
}

HRESULT D3DMeshNode11::InitializeBuffers()
{
	ID3D11Device* device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = m_pLoadedMesh->vertices.size();
	m_indexCount = m_pLoadedMesh->indices.size();

	//create vertex array
	VertexType* vertices = BE_NEW VertexType[m_vertexCount];
	if (!vertices) {
		return false;
	}
	
	int i = 0;
	for (auto vertex = m_pLoadedMesh->vertices.begin(); vertex != m_pLoadedMesh->vertices.end(); vertex++)
	{
		VertexType vert;
		vert.position.x = vertex->pos.x;
		vert.position.y = vertex->pos.y;
		vert.position.z = vertex->pos.z;

		vert.texture.x = vertex->tex.u;
		vert.texture.y = vertex->tex.v;

		vert.normal.x = vertex->normal.x;
		vert.normal.y = vertex->normal.y;
		vert.normal.z = vertex->normal.z;
		vertices[i] = vert;
		i++;
	}

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;

	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	if (FAILED(result))
	{
		return result;
	}

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = m_pLoadedMesh->indices.data();

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
	{
		return result;
	}

	return S_OK;
}

HRESULT D3DMeshNode11::VRender(Scene* pScene)
{
	bool result;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
	ID3D11DeviceContext* context = pRenderer->GetDeviceContext();

	Mat4x4 worldMatrix, viewMatrix, projectionMatrix;
	pRenderer->VGetViewMatrix(viewMatrix);
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetProjectionMatrix(projectionMatrix);

	RenderBuffers(context);

	result = g_pApp->GetGraphicsManager()->GetLightShader()->Render(context, m_indexCount, DirectX::XMLoadFloat4x4(&worldMatrix),
		XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), m_pTexture, XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f));
	if (!result)
	{
		return S_FALSE;
	}

	return S_OK;
}

void D3DMeshNode11::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}