#include "Scene.h"
#include "../BombastEngineStd.h"
#include "../Actor/Actor.h"
#include "../Bombast/BombastApp.h"
#include "../Events/Events.h"
#include "RenderNodes.h"
#include <timeapi.h>

Scene::Scene(IRenderer* renderer)
{
	m_pRoot = BE_NEW RootNode();
	m_pRenderer = renderer;
	m_pLightManager = BE_NEW LightingManager;

	m_pMatrixStack = BE_NEW BMMatrixStack();

	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VAddListener(fastdelegate::MakeDelegate(this, &Scene::NewRenderComponentDelegate), EvtData_New_Render_Component::sEventType);

	Frustum frustum;
	frustum.Init(BE_PI / 4.0f, 1.0f, 1.0f, 100.0f);
	m_pCamera = BE_NEW CameraNode(&Mat4x4::g_Identity, frustum);
	AddChild(INVALID_ACTOR_ID, m_pCamera);
	m_pCamera->SetViewTransform(this);
}

Scene::~Scene()
{
	IEventManager* pEventManager = IEventManager::Get();
	pEventManager->VRemoveListener(fastdelegate::MakeDelegate(this, &Scene::NewRenderComponentDelegate), EvtData_New_Render_Component::sEventType);

	SAFE_DELETE(m_pMatrixStack);
	SAFE_DELETE(m_pLightManager);
	SAFE_DELETE(m_pRoot);
}

HRESULT Scene::DoRender()
{
	if (m_pRoot && m_pCamera)
	{
		m_pCamera->SetViewTransform(this);

		m_pLightManager->CalcLighting(this);

		if (m_pRoot->VPreRender(this) == S_OK)
		{
			OnDeferredRender();
			OnForwardRender();
			m_pRoot->VPostRender(this);
		}
	}

	return S_OK;
}

HRESULT Scene::OnDeferredRender()
{
	//Calls StartDeferredRender!
	g_pApp->GetGraphicsManager()->GetRenderer()->VPrepDeferredRendering();

	m_pRoot->VDeferredRender(this);
	m_pRoot->VDeferredRenderChildren(this);

	return S_OK;
}

HRESULT Scene::OnForwardRender()
{
	//Set RenderTArget back to original render target
	g_pApp->GetGraphicsManager()->GetRenderer()->VPrepForwardRendering();

	//Draw the Deferred Render Buffer first
	g_pApp->GetGraphicsManager()->GetRenderer()->VEnableZBuffer(false);
	g_pApp->GetGraphicsManager()->GetDeferredRenderingManager()->DrawLightPass(g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext(), this);
	g_pApp->GetGraphicsManager()->GetRenderer()->VEnableZBuffer(true);

	m_pRoot->VForwardRender(this);
	m_pRoot->VForwardRenderChildren(this);

	RenderAlphaPass();

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

	LightNode* pLight = static_cast<LightNode*>(child);
	if (pLight != NULL && m_pLightManager->m_lights.size() + 1 < MAX_LIGHTS_SUPPORTED)
	{
		m_pLightManager->m_lights.push_back(pLight);
	}

	return m_pRoot->VAddChild(child);
}

bool Scene::RemoveChild(ActorId id)
{
	if (id == INVALID_ACTOR_ID)
	{
		return false;
	}

	ISceneNode* child = FindActor(id);

	LightNode* pLight = static_cast<LightNode*>(child);
	if (pLight != NULL)
	{
		m_pLightManager->m_lights.remove(pLight);
	}

	m_actorMap.erase(id);
	return m_pRoot->VRemoveChild(id);
}

void Scene::NewRenderComponentDelegate(EventDataPtr pEventData)
{
	std::shared_ptr<EvtData_New_Render_Component> pCastedEventData = std::static_pointer_cast<EvtData_New_Render_Component>(pEventData);

	ActorId actorId = pCastedEventData->GetActorId();
	SceneNode* pSceneNode = pCastedEventData->GetSceneNode();

	if (!pSceneNode)
	{
		return;
	}

	if (FAILED(pSceneNode->VOnRestore(this)))
	{
		BE_ERROR("Failed To Restore scene node to scene for actorid: %u", actorId);
		return;
	}

	AddChild(actorId, pSceneNode);
}

HRESULT Scene::OnUpdate(const float deltaMs)
{
	if (!m_pRoot)
		return S_OK;

	return m_pRoot->VOnUpdate(this, deltaMs);
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