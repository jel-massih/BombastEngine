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
	ReleaseAllShapes();

	SAFE_DELETE(m_pDebugShader);
}

bool DebugPhysics::Initialize()
{
	ID3D11Device* device;
	bool result;

	device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();

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
	ID3D11RasterizerState* rState;
	D3D11_RASTERIZER_DESC rDesc;

	bool result = false;
	ID3D11DeviceContext* deviceContext = g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext();

	IDebugPhysicsRenderBuffer* buffer = g_pApp->GetGameLogic()->VGetGamePhysics()->VGetDebugRenderBuffer();

	//Set Fill Mode to wireframe for spheres
	deviceContext->RSGetState(&rState);
	rState->GetDesc(&rDesc);
	rDesc.FillMode =  D3D11_FILL_WIREFRAME;
	g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice()->CreateRasterizerState(&rDesc, &rState);
	deviceContext->RSSetState(rState);

	ReleaseAllShapes();

	std::vector<IDebugPhysicsShape*> shapes = buffer->VGetDebugShapes();
	for (auto i = shapes.begin(); i != shapes.end(); i++)
	{
		AddShape("TestShape", *i);
	}

	for (int i = 0; i < m_shapeCount; i++)
	{
		result = RenderShape(deviceContext, m_pShapes[i]);
		if (!result)
		{
			return false;
		}
	}

	rDesc.FillMode = D3D11_FILL_SOLID;
	g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice()->CreateRasterizerState(&rDesc, &rState);
	deviceContext->RSSetState(rState);

	//Cleanup Buffer
	SAFE_DELETE(buffer);

	return true;
}

bool DebugPhysics::AddShape(const char* shapeId, IDebugPhysicsShape* shape)
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

	result = InitializeShape(&m_pShapes[m_shapeCount], shapeId, shape->VGetShapeType(), device);
	if (!result)
	{
		return false;
	}

	m_shapeCount++;

	Vec3 color = shape->VGetColor();
	Vec3 pos = shape->VGetPosition();

	m_pShapes[m_shapeCount - 1]->red = color.x;
	m_pShapes[m_shapeCount - 1]->green = color.y;
	m_pShapes[m_shapeCount - 1]->blue = color.z;
	m_pShapes[m_shapeCount - 1]->transformMatrix = shape->VGetTransform();

	SAFE_DELETE(tempShapes);

	return true;
}

bool DebugPhysics::UpdateShape(const char* shapeId, IDebugPhysicsShape* shape)
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

bool DebugPhysics::InitializeShape(DebugShapeType** shape, const char* shapeId, DebugPhysicsShapeType shapeType, ID3D11Device* device)
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

	(*shape)->transformMatrix = Mat4x4::g_Identity;
	(*shape)->vertexBuffer = nullptr;
	(*shape)->indexBuffer = nullptr;
	(*shape)->shapeId = shapeId;

	bool createResult = false;
	switch (shapeType)
	{
	case DebugPhysicsShapeType::SPHERE:
		createResult = CreateSphere(&vertices, &indices, *shape);
		break;
	case DebugPhysicsShapeType::BOX:
		createResult = CreateBox(&vertices, &indices, *shape);
		break;
	}

	if (!createResult)
	{
		BE_ERROR("Failed to Create Debug Physics SHape Buffers");
		return false;
	}

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*shape)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &vertices[0];

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*shape)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*shape)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = &indices[0];
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

bool DebugPhysics::CreateSphere(VertexType** vertices, unsigned long** indices, DebugShapeType* shape)
{
	int lines = 10;

	shape->vertexCount = (lines * lines) * 6;
	shape->indexCount = shape->vertexCount;

	*vertices = BE_NEW VertexType[shape->vertexCount];
	if (!*vertices)
	{
		return false;
	}

	*indices = BE_NEW unsigned long[shape->indexCount];
	if (!*indices)
	{
		return false;
	}

	float X1, Y1, X2, Y2, Z1, Z2;
	float inc1, inc2, inc3, inc4, inc5, Radius1, Radius2;

	int index = 0;

	for (int i = 0; i < lines; i++)
	{
		for (int j = (-lines / 2); j < (lines / 2); j++) 
		{
			inc1 = (i / (float)lines) * 2 * BE_PI;
			inc2 = ((i + 1) / (float)lines) * 2 * BE_PI;
			
			inc3 = (j / (float)lines)*BE_PI;
			inc4 = ((j + 1) / (float)lines)*BE_PI;

			X1 = sin(inc1);
			Y1 = cos(inc1);
			X2 = sin(inc2);
			Y2 = cos(inc2);

			Radius1 = cos(inc3);
			Radius2 = cos(inc4);

			Z1 = sin(inc3);
			Z2 = sin(inc4);

			(*vertices)[index].position = Vec3(Radius1 * X1, Z1, Radius1 * Y1);
			(*vertices)[index + 1].position = Vec3(Radius1*X2, Z1, Radius1*Y2);
			(*vertices)[index + 2].position = Vec3(Radius2*X2, Z2, Radius2*Y2);
			index += 3;

			(*vertices)[index].position = Vec3(Radius1 * X1, Z1, Radius1 * Y1);
			(*vertices)[index + 1].position = Vec3(Radius2*X2, Z2, Radius2*Y2);
			(*vertices)[index + 2].position = Vec3(Radius2*X1, Z2, Radius2*Y1);
			index += 3;
		}
	}

	for (unsigned long i = 0; i < index; i++)
	{
		(*indices)[i] = i;
	}

	return true;
}

bool DebugPhysics::CreateBox(VertexType** vertices, unsigned long** indices, DebugShapeType* shape)
{
	shape->vertexCount = 16;
	shape->indexCount = 36;

	*vertices = BE_NEW VertexType[shape->vertexCount]
	{
		// face vertices
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f) },
		// edge vertices
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f)},
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f)},
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f)},
		{ XMFLOAT3(1.0f, 1.0f, 1.0f)},
		{ XMFLOAT3(1.0f, -1.0f, 1.0f)}
	};

	if (!*vertices)
	{
		return false;
	}

	*indices = BE_NEW unsigned long[shape->indexCount]
	{
		// Front face
		0, 1, 2,
		0, 2, 3,
		// Back face
		4, 6, 5,
		4, 7, 6,
		// Left face
		4, 5, 1,
		4, 1, 0,
		// Right face
		3, 2, 6,
		3, 6, 7,
		// Top face
		1, 5, 6,
		1, 6, 2,
		// Bottom face
		4, 0, 3,
		4, 3, 7
	};

	if (!*indices)
	{
		return false;
	}

	return true;
}

bool DebugPhysics::UpdateShape(DebugShapeType* shape, ID3D11DeviceContext* context)
{
	return false;
}

void DebugPhysics::ReleaseAllShapes()
{
	if (m_pShapes)
	{
		for (int i = 0; i < m_shapeCount; i++)
		{
			ReleaseShape(&m_pShapes[i]);
		}

		SAFE_DELETE(m_pShapes);
		m_shapeCount = 0;
	}
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

bool DebugPhysics::RenderShape(ID3D11DeviceContext* deviceContext, DebugShapeType* shape)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();

	Mat4x4 view, projection;
	pRenderer->VGetViewMatrix(view);
	pRenderer->VGetProjectionMatrix(projection);

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &shape->vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(shape->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pixelColor = XMFLOAT4(shape->red, shape->green, shape->blue, 1.0f);

	result = m_pDebugShader->Render(deviceContext, shape->indexCount, XMLoadFloat4x4(&shape->transformMatrix), XMLoadFloat4x4(&view), XMLoadFloat4x4(&projection), pixelColor);
	if (!result)
	{
		return false;
	}

	return true;
}