#include "Scene.h"
#include "../BombastEngineStd.h"
#include "../Actor/Actor.h"
#include "../Bombast/BombastApp.h"
#include "../Events/Events.h"
#include <timeapi.h>

Scene::Scene(IRenderer* renderer)
{
	m_pRoot = BE_NEW RootNode();
	m_pRenderer = renderer;

	m_pMatrixStack = BE_NEW BMMatrixStack();

	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &Scene::NewRenderComponentDelegate), EvtData_New_Render_Component::sk_EventType);

	Frustum frustum;
	frustum.Init(BE_PI / 4.0f, 1.0f, 1.0f, 100.0f);
	m_pCamera = BE_NEW CameraNode(&Mat4x4::g_Identity, frustum);
	AddChild(INVALID_ACTOR_ID, m_pCamera);
	m_pCamera->SetViewTransform(this);

	m_pGrid = BE_NEW D3D11GridNode(INVALID_ACTOR_ID, nullptr, &Mat4x4::g_Identity);
	AddChild(INVALID_ACTOR_ID, m_pGrid);
	m_pGrid->SetVisible(false);
}

Scene::~Scene()
{
	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &Scene::NewRenderComponentDelegate), EvtData_New_Render_Component::sk_EventType);

	SAFE_DELETE(m_pMatrixStack);
	
	SAFE_DELETE(m_pRoot);
}

HRESULT Scene::OnRender()
{
	if (m_pRoot && m_pCamera)
	{
		m_pCamera->SetViewTransform(this);

		if (m_pRoot->VPreRender(this) == S_OK)
		{
			m_pRoot->VRender(this);
			m_pRoot->VRenderChildren(this);
			m_pRoot->VPostRender(this);
		}

		RenderAlphaPass();
	}

	return S_OK;
}

HRESULT Scene::OnLostDevice()
{
	if (m_pRoot)
	{
		m_pRoot->VOnLostDevice(this);
	}
	return S_OK;
}

HRESULT Scene::OnRestore()
{
	if (!m_pRoot)
		return S_OK;

	HRESULT hr;
	BE_HRETURN(m_pRenderer->VOnRestore(), "Failed to restore Renderer");

	return m_pRoot->VOnRestore(this);
}

bool Scene::AddChild(ActorId id, ISceneNode* child)
{
	if (id != INVALID_ACTOR_ID)
	{
		m_actorMap[id] = child;
	}

	//@TODO: Add Lights

	return m_pRoot->VAddChild(child);
}

bool Scene::RemoveChild(ActorId id)
{
	if (id == INVALID_ACTOR_ID)
	{
		return false;
	}

	ISceneNode* child = FindActor(id);

	//@TODO: Remove Light

	m_actorMap.erase(id);
	return m_pRoot->VRemoveChild(id);
}

void Scene::NewRenderComponentDelegate(IEventDataPtr pEventData)
{
	std::shared_ptr<EvtData_New_Render_Component> pCastedEventData = std::static_pointer_cast<EvtData_New_Render_Component>(pEventData);

	ActorId actorId = pCastedEventData->GetActorId();
	SceneNode* pSceneNode = pCastedEventData->GetSceneNode();

	if (!pSceneNode)
	{
		BE_ERROR("Failed To get scene node for actorid: " + ToStr(actorId));
		return;
	}

	if (FAILED(pSceneNode->VOnRestore(this)))
	{
		BE_ERROR("Failed To Restore scene node to scene for actorid: " + ToStr(actorId));
		return;
	}

	AddChild(actorId, pSceneNode);
}

HRESULT Scene::OnUpdate(const int deltaMS)
{
	if (!m_pRoot)
		return S_OK;

	static DWORD lastTime = timeGetTime();
	DWORD elapsedMs = 0;
	DWORD now = timeGetTime();

	elapsedMs = now - lastTime;
	lastTime = now;

	return m_pRoot->VOnUpdate(this, elapsedMs);
}

ISceneNode* Scene::FindActor(ActorId id)
{
	SceneActorMap::iterator i = m_actorMap.find(id);
	if (i == m_actorMap.end())
	{
		return nullptr;
	}

	return i->second;
}

void Scene::RenderAlphaPass()
{
//	IRenderState* alphaPass = m_pRenderer->VPrepareAlphaPass();
	
	//@TODO: Alpha Pass Stuff
}