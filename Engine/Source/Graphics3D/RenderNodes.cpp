#include "RenderNodes.h"
#include "Lighting.h"
#include "../Resources/ModelResource.h"
#include "../Resources/MaterialResource.h"

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

HRESULT D3DBitmapNode11::VForwardRender(Scene* pScene)
{
	HRESULT hr;

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
	/*result = g_pApp->GetGraphicsManager()->GetShaderManager()->RenderRenderable(this, m_indexCount, pScene);
	if (!result)
	{
		return S_FALSE;
	}*/

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

	m_material = BE_NEW Material;
	m_material->SetShaderType(BSHADER_TYPE_COLOR);
	m_material->SetDiffuse(Vec4(1, 1, 1, 1));
}

D3D11GridNode::~D3D11GridNode()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_DELETE(m_material);
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

HRESULT D3D11GridNode::VForwardRender(Scene* pScene)
{
	bool result;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
	ID3D11DeviceContext* context = pRenderer->GetDeviceContext();

	RenderBuffers(context);

	result = g_pApp->GetGraphicsManager()->GetShaderManager()->RenderRenderable(this, m_material, m_indexCount, pScene);
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

MeshNode::MeshNode(const ActorId actorId, BaseRenderComponent* renderComponent, std::string meshFileName, RenderPass renderPass, const Mat4x4 *t)
	: SceneNode(actorId, renderComponent, renderPass, t), m_meshFilename(meshFileName)
{
}

D3DMeshNode11::D3DMeshNode11(const ActorId actorId, BaseRenderComponent* renderComponent, std::string meshFilename, std::vector<std::string> materialsFilenames, RenderPass renderPass, const Mat4x4* t)
	: MeshNode(actorId, renderComponent, meshFilename, renderPass, t), m_lastPosition(Vec3::g_InvalidVec3), m_materialFilenames(materialsFilenames)
{
	m_pLoadedMesh = nullptr;
}

D3DMeshNode11::~D3DMeshNode11()
{
	m_submeshBuffers.clear();
}

HRESULT D3DMeshNode11::VOnRestore(Scene* pScene)
{
	HRESULT hr;

	BE_HRETURN(SceneNode::VOnRestore(pScene), "Failed to restore scenenode for D3DMeshNode11");

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

	return S_OK;
}

Material* D3DMeshNode11::LoadMaterial(std::string filename)
{
	return MaterialResourceLoader::LoadAndReturnMaterialResource(filename.c_str());
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
	HRESULT result;

	m_submeshBuffers.clear();

	ID3D11Device* device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();
	for (unsigned int i = 0; i < m_pLoadedMesh->meshes.size(); i++)
	{
		SubMeshBuffers submeshBuffer;
		
		submeshBuffer.material = nullptr;

		if (m_pLoadedMesh->meshes[i].materialIndex < m_materialFilenames.size()) {
			submeshBuffer.material = LoadMaterial(m_materialFilenames[m_pLoadedMesh->meshes[i].materialIndex]);
		}

		if (submeshBuffer.material == nullptr)
		{
			submeshBuffer.material = MaterialResourceLoader::LoadAndReturnDefaultMaterialResource();
		}

		//If Material has non-standard UV Scale, then update uvs
		XMFLOAT2 uvScale = submeshBuffer.material->GetUVScale();
		if (fabs(uvScale.x - 1.0f) > 0.0005 || fabs(uvScale.y - 1.0f) > 0.0005)
		{
			for (auto it = m_pLoadedMesh->meshes[i].vertices.begin(); it != m_pLoadedMesh->meshes[i].vertices.end(); it++)
			{
				(*it).tex.u *= uvScale.x;
				(*it).tex.v *= uvScale.x;
			}
		}


		result = InitializeSubMeshBuffers(m_pLoadedMesh->meshes[i], &submeshBuffer.pVertexBuffer, &submeshBuffer.pIndexBuffer);
		if (FAILED(result)) {
			return false;
		}

		submeshBuffer.vertexCount = (unsigned int)m_pLoadedMesh->meshes[i].vertices.size();
		submeshBuffer.indexCount = (unsigned int)m_pLoadedMesh->meshes[i].indices.size();

		m_submeshBuffers.push_back(submeshBuffer);
	}

	return S_OK;
}

HRESULT D3DMeshNode11::InitializeSubMeshBuffers(const ModelClass::SubMesh& submesh, ID3D11Buffer** pVertexBuffer, ID3D11Buffer** pIndexBuffer)
{
	ID3D11Device* device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	unsigned int vertCount = (unsigned int)submesh.vertices.size();
	unsigned int indexCount = (unsigned int)submesh.indices.size();

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ModelClass::BasicVertex) * vertCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;

	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = submesh.vertices.data();

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, pVertexBuffer);
	if (FAILED(result))
	{
		return result;
	}

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = submesh.indices.data();

	result = device->CreateBuffer(&indexBufferDesc, &indexData, pIndexBuffer);
	if (FAILED(result))
	{
		return result;
	}

	return S_OK;
}

HRESULT D3DMeshNode11::VDeferredRender(Scene* pScene)
{
	bool result;

	ID3D11DeviceContext* context = g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext();

	result = RenderDeferredBuffers(context);
	if (!result) {
		return S_FALSE;
	}

	return S_OK;
}

bool D3DMeshNode11::RenderDeferredBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	for (auto it = m_submeshBuffers.begin(); it != m_submeshBuffers.end(); it++)
	{
		if ((*it).material->GetShaderType() != BSHADER_TYPE_DEFERRED_LIT)
		{
			continue;
		}

		deviceContext->IASetVertexBuffers(0, 1, &(*it).pVertexBuffer, &stride, &offset);

		deviceContext->IASetIndexBuffer((*it).pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
		Mat4x4 worldMatrix, viewMatrix, projectionMatrix;
		pRenderer->VGetViewMatrix(viewMatrix);
		pRenderer->VGetWorldMatrix(worldMatrix);
		pRenderer->VGetProjectionMatrix(projectionMatrix);

		if ((*it).material->GetTextures().size() > 0) {
			g_pApp->GetGraphicsManager()->GetDeferredRenderingManager()->DrawRenderable(deviceContext, (*it).indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), (*it).material->GetTextures().front()->GetTexture());
		}
	}

	return true;
}

HRESULT D3DMeshNode11::VForwardRender(Scene* pScene)
{
	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
	ID3D11DeviceContext* context = pRenderer->GetDeviceContext();

	if (!RenderForwardBuffers(context, pScene))
	{
		return S_FALSE;
	}
	
	return S_OK;
}

bool D3DMeshNode11::RenderForwardBuffers(ID3D11DeviceContext* deviceContext, Scene* pScene)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	for (auto it = m_submeshBuffers.begin(); it != m_submeshBuffers.end(); it++)
	{
		if ((*it).material->GetShaderType() != BSHADER_TYPE_DEFERRED_LIT)
		{
			deviceContext->IASetVertexBuffers(0, 1, &(*it).pVertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer((*it).pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		if (!g_pApp->GetGraphicsManager()->GetShaderManager()->RenderRenderable(this, (*it).material, (*it).indexCount, pScene))
		{
			return false;
		}
	}

	return true;
}