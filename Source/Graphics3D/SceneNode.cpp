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
		mat.SetPosition(Vec3(0.0f, 0.0f, -10.0f));
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