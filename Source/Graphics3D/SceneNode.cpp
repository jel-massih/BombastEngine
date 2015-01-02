#include "SceneNode.h"
#include "../BombastEngineStd.h"
#include "../Actor/RenderComponent.h"
#include "../Actor/TransformComponent.h"
#include "../Bombast/BombastApp.h"
#include "Scene.h"

SceneNodeProperties::SceneNodeProperties()
{
	m_actorId = INVALID_ACTOR_ID;
	m_radius = 0;
	m_renderPass = RenderPass_0;
	m_alphaType = AlphaOpaque;

	m_toWorld = Mat4x4::g_Identity;
	m_fromWorld = Mat4x4::g_Identity.Inverse();
}

void SceneNodeProperties::Transform(Mat4x4* toWorld, Mat4x4* fromWorld) const
{
	if (toWorld)
	{
		*toWorld = m_toWorld;
	}
	
	if (fromWorld)
	{
		*fromWorld = m_fromWorld;
	}
}

SceneNode::SceneNode(ActorId actorId, BaseRenderComponent* renderComponent, RenderPass renderPass, const Mat4x4* to, const Mat4x4* from)
{
	m_pParent = NULL;
	m_properties.m_actorId = actorId;
	m_properties.m_name = (renderComponent) ? renderComponent->VGetName(): "SceneNode";
	m_properties.m_renderPass = renderPass;
	m_properties.m_alphaType = AlphaOpaque;
	m_pRenderComponent = renderComponent;
	VSetTransform(to, from);
	SetRadius(0);
}

SceneNode::~SceneNode()
{
	if (!m_children.empty())
	{
		for (auto &it : m_children)
		{
			SAFE_DELETE(it);
		}

		m_children.clear();
	}
}

HRESULT SceneNode::VOnRestore(Scene* pScene)
{
	Color color = (m_pRenderComponent) ? m_pRenderComponent->GetColor() : g_White;
	m_properties.m_material.SetDiffuse(color);

	SceneNodeList::iterator i = m_children.begin();
	SceneNodeList::iterator end = m_children.end();
	while (i != end)
	{
		(*i)->VOnRestore(pScene);
		i++;
	}

	return S_OK;
}

HRESULT SceneNode::VOnLostDevice(Scene* pScene)
{
	SceneNodeList::iterator i = m_children.begin();
	SceneNodeList::iterator end = m_children.end();
	while (i != end)
	{
		(*i)->VOnLostDevice(pScene);
		i++;
	}

	return S_OK;
}

void SceneNode::VSetTransform(const Mat4x4* toWorld, const Mat4x4* fromWorld)
{
	m_properties.m_toWorld = *toWorld;
	if (!fromWorld)
	{
		m_properties.m_fromWorld = m_properties.m_toWorld.Inverse();
	}
	else
	{
		m_properties.m_fromWorld = *fromWorld;
	}
}

HRESULT SceneNode::VPreRender(Scene* pScene)
{
	Actor* pActor = g_pApp->GetGameLogic()->VGetActor(m_properties.GetActorId());
	if (pActor)
	{
		TransformComponent* pTransform = pActor->GetComponent<TransformComponent>(TransformComponent::g_Name);
		if (pTransform)
		{
			m_properties.m_toWorld = pTransform->GetTransform();
		}
	}

	pScene->PushAndSetMatrix(m_properties.m_toWorld);

	return S_OK;
}

HRESULT SceneNode::VPostRender(Scene* pScene)
{
	pScene->PopMatrix();
	return S_OK;
}

bool SceneNode::VIsVisible(Scene* pScene) const
{
	Mat4x4 toWorld, fromWorld;

	pScene->GetCamera()->VGet()->Transform(&toWorld, &fromWorld);

	Vec3 pos = GetWorldPosition();
	Vec3 fromWorldPos = fromWorld.Xform(pos);

	Frustum const &frustum = pScene->GetCamera()->GetFrustum();

	bool isVisibile = frustum.Inside(fromWorldPos, VGet()->GetRadius());

	return isVisibile;
}

const Vec3 SceneNode::GetWorldPosition() const
{
	Vec3 pos = GetPosition();
	if (m_pParent)
	{
//		pos += m_pParent->GetWorldPosition();
	}

	return pos;
}

HRESULT SceneNode::VOnUpdate(Scene* pScene, DWORD const elapsedMs)
{
	SceneNodeList::iterator i = m_children.begin();
	SceneNodeList::iterator end = m_children.end();

	while (i != end)
	{
		(*i)->VOnUpdate(pScene, elapsedMs);
		i++;
	}

	return S_OK;
}

HRESULT SceneNode::VRenderChildren(Scene* pScene)
{
	SceneNodeList::iterator i = m_children.begin();
	SceneNodeList::iterator end = m_children.end();

	while (i != end)
	{
		if ((*i)->VPreRender(pScene) == S_OK)
		{
			if ((*i)->VIsVisible(pScene))
			{
				float alpha = (*i)->VGet()->m_material.GetAlpha();
				if (alpha == fOPAQUE)
				{
					(*i)->VRender(pScene);
				}
				else if (alpha != fTRANSPARENT)
				{
					//@TODO: Render Transparent Crap
				}
			}
			(*i)->VRenderChildren(pScene);
		}
		(*i)->VPostRender(pScene);
		i++;
	}

	return S_OK;
}

bool SceneNode::VAddChild(ISceneNode* ichild)
{
	m_children.push_back(ichild);

	SceneNode* child = static_cast<SceneNode*>(ichild);

	child->m_pParent = this;

	Vec3 childPos = child->VGet()->ToWorld().GetPosition();

	float newRadius = childPos.Length() + child->VGet()->GetRadius();

	if (newRadius > m_properties.m_radius)
	{
		m_properties.m_radius = newRadius;
	}

	return true;
}

bool SceneNode::VRemoveChild(ActorId actorId)
{
	for (SceneNodeList::iterator it = m_children.begin(); it != m_children.end(); it++)
	{
		const SceneNodeProperties* pProps = (*it)->VGet();
		if (pProps->GetActorId() != INVALID_ACTOR_ID && actorId == pProps->GetActorId())
		{
			it = m_children.erase(it);
		}
	}
	return false;
}

HRESULT SceneNode::VPick(Scene* pScene, RayCast* raycast)
{
	for (SceneNodeList::iterator it = m_children.begin(); it != m_children.end(); it++)
	{
		HRESULT hr = (*it)->VPick(pScene, raycast);

		if (hr == E_FAIL)
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

void SceneNode::SetAlpha(float alpha)
{
	m_properties.SetAlpha(alpha);
	for (SceneNodeList::const_iterator i = m_children.begin(); i != m_children.end(); i++)
	{
		SceneNode* sceneNode = static_cast<SceneNode*>(*i);
		sceneNode->SetAlpha(alpha);
	}
}

RootNode::RootNode() : SceneNode(INVALID_ACTOR_ID, nullptr, RenderPass_0, &Mat4x4::g_Identity)
{
	m_children.reserve(RenderPass_Last);

	SceneNode* staticGroup = BE_NEW SceneNode(INVALID_ACTOR_ID, nullptr, RenderPass_Static, &Mat4x4::g_Identity);
	m_children.push_back(staticGroup);

	SceneNode* actorGroup = BE_NEW SceneNode(INVALID_ACTOR_ID, nullptr, RenderPass_Actor, &Mat4x4::g_Identity);
	m_children.push_back(actorGroup);

	SceneNode* guiGroup = BE_NEW SceneNode(INVALID_ACTOR_ID, nullptr, RenderPass_GUI, &Mat4x4::g_Identity);
	m_children.push_back(guiGroup);

	SceneNode* skyGroup = BE_NEW SceneNode(INVALID_ACTOR_ID, nullptr, RenderPass_Sky, &Mat4x4::g_Identity);
	m_children.push_back(skyGroup);

	SceneNode* invisibleGroup = BE_NEW SceneNode(INVALID_ACTOR_ID, nullptr, RenderPass_NotRendered, &Mat4x4::g_Identity);
	m_children.push_back(invisibleGroup);
}

bool RootNode::VAddChild(ISceneNode* child)
{
	RenderPass pass = child->VGet()->GetRenderPass();
	if (static_cast<unsigned>(pass) >= m_children.size() || !m_children[pass])
	{
		BE_ASSERT(0 && _T("There is no such render pass"));
		return false;
	}

	return m_children[pass]->VAddChild(child);
}

bool RootNode::VRemoveChild(ActorId id)
{
	bool bRemoved = false;
	for (int i = RenderPass_0; i < RenderPass_Last; i++)
	{
		if (m_children[i]->VRemoveChild(id))
		{
			bRemoved = true;
		}
	}

	return bRemoved;
}

HRESULT RootNode::VRenderChildren(Scene* pScene)
{
	for (int pass = RenderPass_0; pass < RenderPass_Last; pass++)
	{
		switch (pass)
		{
		case RenderPass_Static:
		case RenderPass_Actor:
			m_children[pass]->VRenderChildren(pScene);
			break;
		case RenderPass_GUI:
			pScene->GetRenderer()->VEnableZBuffer(false);
			m_children[pass]->VRenderChildren(pScene);
			pScene->GetRenderer()->VEnableZBuffer(true);
			break;
		case RenderPass_Sky:
//			IRenderState* skyPass = pScene->GetRenderer()->VPrepareSkyBoxPass();
			m_children[pass]->VRenderChildren(pScene);
			break;
		}
	}

	return S_OK;
}

HRESULT CameraNode::VRender(Scene* pScene)
{
	if (m_bDebugCamera)
	{
		pScene->PopMatrix();
		m_frustum.Render();

		pScene->PushAndSetMatrix(m_properties.ToWorld());
	}

	return S_OK;
}

HRESULT CameraNode::VOnRestore(Scene* pScene)
{
	Point screenSize = g_pApp->GetScreenSize();
	m_frustum.SetAspect(screenSize.GetX() / (FLOAT)screenSize.GetY());
	XMStoreFloat4x4(&m_projection, XMMatrixPerspectiveFovLH(m_frustum.m_fov, m_frustum.m_aspect, m_frustum.m_near, m_frustum.m_far));
	pScene->GetRenderer()->VSetProjectionTransform(&m_projection);
	return S_OK;
}

HRESULT CameraNode::SetViewTransform(Scene* pScene)
{
	if (m_pTarget)
	{
		Mat4x4 mat = m_pTarget->VGet()->ToWorld();
		Vec4 at = m_camOffsetVector;
		Vec4 atWorld = mat.Xform(at);
		Vec3 pos = mat.GetPosition() + Vec3(atWorld);
		mat.SetPosition(pos);
		VSetTransform(&mat);
	}
	else
	{
		Mat4x4 mat = VGet()->ToWorld();
		mat.SetPosition(Vec3(g_pApp->GetScreenSize().GetX() / 2, -(g_pApp->GetScreenSize().GetY() / 2), -50.0l));
		VSetTransform(&mat);
	}

	m_view = VGet()->FromWorld();

	pScene->GetRenderer()->VSetViewTransform(&m_view);

	return S_OK;
}

Mat4x4 CameraNode::GetWorldViewProjection(Scene* pScene)
{
	Mat4x4 world = pScene->GetTopMatrix();
	Mat4x4 view = VGet()->FromWorld();
	Mat4x4 worldView = world * view;
	return worldView * m_projection;
}

HRESULT CameraNode::VOnUpdate(Scene* pScene, DWORD const elapsedMs)
{
	return S_OK;
}

D3D11GridNode::D3D11GridNode(ActorId actorId, BaseRenderComponent* renderComponent, const Mat4x4* pMatrix)
	: SceneNode(actorId, renderComponent, RenderPass_0, pMatrix)
{
	m_vertCount = m_indexCount = 0;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;

	m_gridWidth = 100;
	m_gridHeight = 100;
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

	if (!InitializeBuffers(g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice()))
	{
		return S_FALSE;
	}

	return S_OK;
}

HRESULT D3D11GridNode::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	float posX, posZ;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertCount = (m_gridWidth) * (m_gridHeight) * 8;
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
			posZ = (float)(i+1);

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
	unsigned int stride;
	unsigned int offset;

	ID3D11DeviceContext* context = g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext();

	stride = sizeof(VertexType);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_pVertexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	return S_OK;
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

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	if (m_primitiveType == PrimitiveType::PT_Box)
	{
		m_vertCount = 8;
		m_indexCount = m_vertCount;

		vertices = BE_NEW VertexType[m_vertCount];
		if (!vertices)
		{
			return S_FALSE;
		}

		indices = BE_NEW unsigned long[m_indexCount];
		if (!indices)
		{
			return S_FALSE;
		}
		
		memset(vertices, 0, sizeof(VertexType) * m_vertCount);

		for (i = 0; i < m_indexCount; i++)
		{
			indices[i] = i;
		}

		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
			return result;
		}

		SAFE_DELETE_ARRAY(vertices);
		SAFE_DELETE_ARRAY(indices);
	}

	return S_OK;
}

HRESULT D3D11PrimitiveNode::VRender(Scene* pScene)
{
	HRESULT hr;
	bool result;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
	ID3D11DeviceContext* context = pRenderer->GetDeviceContext();

	Mat4x4 worldMatrix, viewMatrix, orthoMatrix;
	pRenderer->VGetViewMatrix(viewMatrix);
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetOrthoMatrix(orthoMatrix);

	hr = UpdateBuffers(context);
	if (FAILED(hr))
	{
		return hr;
	}

	RenderBuffers(context);

	result = g_pApp->GetGraphicsManager()->GetTextureShader()->Render(context, m_indexCount, DirectX::XMLoadFloat4x4(&worldMatrix),
		XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&orthoMatrix), m_pTexture);
	if (!result)
	{
		return S_FALSE;
	}

	return S_OK;
}

HRESULT D3D11PrimitiveNode::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	float radius = 10.0f;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* vertexPtr;
	HRESULT result;

	if (VGet()->ToWorld().GetPosition() == m_lastPos)
	{
		return S_OK;
	}
	m_lastPos = VGet()->ToWorld().GetPosition();

	vertices = BE_NEW VertexType[m_vertCount];
	if (!vertices)
	{
		return false;
	}

	vertices[0].position = DirectX::XMFLOAT3(-radius, radius, -radius);
	
	vertices[1].position = XMFLOAT3(radius, radius, -radius);

	vertices[2].position = XMFLOAT3(-radius, -radius, -radius);

	vertices[3].position = XMFLOAT3(radius, -radius, -radius);

	vertices[4].position = XMFLOAT3(-radius, radius, radius);

	vertices[5].position = XMFLOAT3(radius, radius, radius);
	
	vertices[6].position = XMFLOAT3(-radius, -radius, radius);

	vertices[7].position = XMFLOAT3(radius, -radius, radius);

	result = deviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return result;
	}

	vertexPtr = (VertexType*)mappedResource.pData;

	memcpy(vertexPtr, (void*)vertices, sizeof(VertexType) * m_vertCount);

	deviceContext->Unmap(m_pVertexBuffer, 0);
	
	SAFE_DELETE_ARRAY(vertices);
	
	return S_OK;
}

void D3D11PrimitiveNode::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();

	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}