#include "DebugPhysics.h"
#include "../Shading/DebugShader.h"

DebugPhysics::DebugPhysics()
{
	m_pDebugShader = nullptr;
	m_pShapes = nullptr;
	m_shapeCount = 0;
}

DebugPhysics::~DebugPhysics()
{
	if (m_pShapes)
	{
		for (int i = 0; i < m_shapeCount; i++)
		{
			ReleaseShape(&m_pShapes[i]);
		}

		SAFE_DELETE(m_pShapes);
	}

	SAFE_DELETE(m_pDebugShader);
}

bool DebugPhysics::Initialize()
{
	ID3D11Device* device;
	bool result;

	device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();

	g_pApp->GetGraphicsManager()->GetRenderer()->VGetViewMatrix(m_baseViewMatrix);

	m_pDebugShader = BE_NEW DebugShader;
	if (!m_pDebugShader)
	{
		return false;
	}

	result = m_pDebugShader->Initialize(device);
	if (!result)
	{
		BE_ERROR("Could not initialize the Debug Shader Object");
		return false;
	}

	return true;
}

bool DebugPhysics::Render()
{
	bool result = false;
	ID3D11DeviceContext* deviceContext = g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext();

	Mat4x4 worldMatrix, orthoMatrix;

	g_pApp->GetGraphicsManager()->GetRenderer()->VGetWorldMatrix(worldMatrix);
	g_pApp->GetGraphicsManager()->GetRenderer()->VGetOrthoMatrix(orthoMatrix);

	IDebugPhysicsRenderBuffer* buffer = g_pApp->m_pGame->VGetGamePhysics()->VGetDebugRenderBuffer();

	std::vector<IDebugPhysicsSphere*> spheres = buffer->VGetDebugSpheres();

	for (unsigned int i = 0; i < spheres.size(); i++)
	{
		//result = RenderSphere(deviceContext, spheres[i], worldMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}
	}

	return true;
}

bool DebugPhysics::AddShape(const char* shapeId, IDebugPhysicsSphere* shape)
{
	bool result;
	DebugShapeType** tempShapes;

	ID3D11Device* device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();
	tempShapes = m_pShapes;

	m_pShapes = BE_NEW DebugShapeType*[m_shapeCount + 1];
	if (!m_pShapes)
	{
		return false;
	}

	for (int i = 0; i < m_shapeCount; i++)
	{
		m_pShapes[i] = tempShapes[i];
	}

	result = InitializeShape(&m_pShapes[m_shapeCount], shapeId, device);
	if (!result)
	{
		return false;
	}

	m_shapeCount++;

	SAFE_DELETE(tempShapes);

	return true;
}

bool DebugPhysics::UpdateShape(const char* shapeId, IDebugPhysicsSphere* shape)
{
	bool result;

	ID3D11DeviceContext* context = g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext();

	for (int i = 0; i < m_shapeCount; i++)
	{
		if (m_pShapes[i]->shapeId == shapeId)
		{
			result = UpdateShape(m_pShapes[i], context);
			if (!result)
			{
				return false;
			}
		}
	}

	return true;
}

bool DebugPhysics::InitializeShape(DebugShapeType** shape, const char* shapeId, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	*shape = BE_NEW DebugShapeType;
	if (!*shape)
	{
		return false;
	}

	(*shape)->vertexBuffer = 0;
	(*shape)->indexBuffer = 0;
	(*shape)->vertexCount = 0;
	(*shape)->indexCount = (*shape)->vertexCount;
	(*shape)->shapeId = shapeId;

	vertices = BE_NEW VertexType[(*shape)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = BE_NEW unsigned long[(*shape)->indexCount];
	if (!indices)
	{
		return false;
	}

	memset(vertices, 0, sizeof(VertexType) * (*shape)->vertexCount);

	for (int i = 0; i < (*shape)->indexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*shape)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*shape)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*shape)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*shape)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	return true;
}

bool DebugPhysics::UpdateShape(DebugShapeType* shape, ID3D11DeviceContext* context)
{
	return false;
}

void DebugPhysics::ReleaseShape(DebugShapeType** shape)
{
	if (*shape)
	{
		SAFE_RELEASE((*shape)->vertexBuffer);
		SAFE_RELEASE((*shape)->indexBuffer);
		
		SAFE_DELETE(*shape);
	}
}

bool DebugPhysics::RenderShape(ID3D11DeviceContext* deviceContext, DebugShapeType* shape, Mat4x4& worldMatrix, Mat4x4& orthoMatrix)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &shape->vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(shape->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pixelColor = XMFLOAT4(shape->red, shape->green, shape->blue, 1.0f);

	result = m_pDebugShader->Render(deviceContext, shape->indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&m_baseViewMatrix), XMLoadFloat4x4(&orthoMatrix), pixelColor);
	if (!result)
	{
		return false;
	}

	return true;
}