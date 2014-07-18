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

	//@TODO: 
//	pScene->PushAndSetMatrix(m_properties.m_toWorld);

	return S_OK;
}

HRESULT SceneNode::VPostRender(Scene* pScene)
{
	//@TODO:
	//pScene->PopMatrix();
	return S_OK;
}

bool SceneNode::VIsVisible(Scene* pScene) const
{
	Mat4x4 toWorld, fromWorld;

	//@TODO:
	//pScene->GetCamera()->VGet()->Transform(&toWorld, &fromWorld);

	Vec3 pos = GetWorldPosition();
	Vec3 fromWorldPos = fromWorld.Xform(pos);

	//@TODO:
	//Frustum const &frustum = pScene->GetCamera()->GetFrustum();

	//bool isVisibile = frustum.Inside(fromWorldPos, VGet()->GetRadius());

	//return isVisibile;
	return true;
}

const Vec3 SceneNode::GetWorldPosition() const
{
	Vec3 pos = GetPosition();
	if (m_pParent)
	{
		//@TODO:
		//pos += m_pParent->GetWorldPosition();
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
	return S_OK;
}

bool SceneNode::VAddChild(ISceneNode* kid)
{
	return true;
}

bool SceneNode::VRemoveChild(ActorId actorId)
{
	return false;
}

HRESULT SceneNode::VPick(Scene* pScene, RayCast* raycast)
{
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