#include "SceneNode.h"
#include "../BombastEngineStd.h"
#include "../Actor/RenderComponent.h"
#include "../Actor/TransformComponent.h"
#include "../Bombast/BombastApp.h"

SceneNodeProperties::SceneNodeProperties()
{
	m_actorId = INVALID_ACTOR_ID;
	m_radius = 0;
	m_renderPass = RenderPass_0;
	m_alphaType = AlphaOpaque;
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
	Actor* pActor = BombastApp::GetGameInstance()->GetGameLogic()->VGetActor(m_properties.GetActorId());
	if (pActor)
	{
		TransformComponent* pTransform = pActor->GetComponent<TransformComponent>(TransformComponent::g_Name);
		if (pTransform)
		{
			m_properties.m_toWorld = pTransform->GetTransform();
		}
	}

//	pScene->PushAndSetMatrix

	return S_OK;
}