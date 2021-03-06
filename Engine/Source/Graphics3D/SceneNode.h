#pragma once

#include "../Actor/Actor.h"
#include "Geometry.h"
#include "Material.h"
#include <vector>

class SceneNode;
class BaseRenderComponent;

//Defines available types of Alpha BLending
enum AlphaType
{
	AlphaOpaque,
	AlphaTexture,
	AlphaMaterial,
	AlphaVertex
};

//Contained in SceneNode class to provide easy accessor
class SceneNodeProperties
{
	friend class SceneNode;
public:
	SceneNodeProperties();
	const ActorId& GetActorId() const { return m_actorId; }
	Mat4x4 const& ToWorld() const { return m_toWorld; }
	Mat4x4 const& FromWorld() const { return m_fromWorld; }
	void Transform(Mat4x4* toWorld, Mat4x4* fromWorld) const;

	const char* GetName() const { return m_name.c_str(); }

	AlphaType GetAlphaType() const { return m_alphaType; }

	RenderPass GetRenderPass() const { return m_renderPass; }
	float GetRadius() const { return m_radius; }

protected:
	ActorId m_actorId;
	std::string m_name;
	Mat4x4 m_toWorld, m_fromWorld;
	float m_radius;
	RenderPass m_renderPass;
	AlphaType m_alphaType;
};

typedef std::vector<ISceneNode*> SceneNodeList;

class SceneNode : public ISceneNode
{
	friend class Scene;

public:
	SceneNode(ActorId actorId, BaseRenderComponent* renderComponent, RenderPass renderPass, const Mat4x4* to, const Mat4x4* from=NULL);

	virtual ~SceneNode();

	virtual const SceneNodeProperties* const VGet() const { return &m_properties; }

	virtual void VSetTransform(const Mat4x4* toWorld, const Mat4x4* fromWorld = NULL);
	
	virtual HRESULT VOnRestore(Scene* pScene);
	virtual HRESULT VOnUpdate(Scene* pScene, const float deltaMs);

	virtual bool VIsVisible(Scene* pScene) const;

	virtual HRESULT VPreRender(Scene* pScene);
	virtual HRESULT VPostRender(Scene* pScene);

	virtual HRESULT VDeferredRender(Scene* pScene) { return S_OK; }
	virtual HRESULT VDeferredRenderChildren(Scene* pScene);

	virtual HRESULT VForwardRender(Scene* pScene) { return S_OK; }
	virtual HRESULT VForwardRenderChildren(Scene* pScene);

	virtual bool VAddChild(ISceneNode* kid);
	virtual bool VRemoveChild(ActorId id);
	virtual HRESULT VOnLostDevice(Scene* pScene);
	virtual HRESULT VPick(Scene* pScene, RayCast* pRayCast);

	void SetAlpha(float alpha);

	Vec3 GetPosition() const { return m_properties.m_toWorld.GetPosition(); }
	void SetPosition(const Vec3& pos) { m_properties.m_toWorld.SetPosition(pos); m_properties.m_fromWorld = m_properties.m_toWorld.Inverse(); }

	const Vec3 GetWorldPosition() const;

	Vec3 GetDirection() const { return m_properties.m_toWorld.GetDirection(); }

	void SetRadius(const float radius) { m_properties.m_radius = radius; }

protected:
	SceneNodeList m_children;
	SceneNode* m_pParent;
	SceneNodeProperties m_properties;
	BaseRenderComponent* m_pRenderComponent;
};

class RootNode : public SceneNode
{
public:
	RootNode();
	virtual bool VAddChild(ISceneNode* child);
	virtual HRESULT VDeferredRenderChildren(Scene* pScene);
	virtual HRESULT VForwardRenderChildren(Scene* pScene);
	virtual bool VRemoveChild(ActorId id);
	virtual bool VIsVisible(Scene* pScene) const { return true; }
};

class CameraNode : public SceneNode
{
public :
	CameraNode(Mat4x4 const* t, Frustum const& frustum)
		:SceneNode(INVALID_ACTOR_ID, nullptr, RenderPass_0, t),
		m_frustum(frustum),
		m_bActive(true),
		m_bDebugCamera(false),
		m_pFollowTarget(nullptr),
		m_pViewTarget(nullptr),
		m_camOffsetVector(0.0f, 0.0f, 0.0f, 0.0f)
	{
	}

	virtual HRESULT VForwardRender(Scene* pScene);
	virtual HRESULT VOnRestore(Scene* pScene);
	virtual HRESULT VOnUpdate(Scene* pScene, const float deltaMs);
	virtual bool VIsVisible(Scene* pScene) const { return m_bActive; }

	const Frustum& GetFrustum() const { return m_frustum; }
	
	void SetFollowTarget(SceneNode* pTarget) { m_pFollowTarget = pTarget; }
	void ClearFollowTarget() { m_pFollowTarget = nullptr; }
	SceneNode* GetFollowTarget() { return m_pFollowTarget; }

	void SetViewTarget(SceneNode* pTarget) { m_pViewTarget = pTarget; }
	void ClearViewTarget() { m_pViewTarget = nullptr; }
	SceneNode* GetViewTarget() { return m_pViewTarget; }

	Mat4x4 GetWorldViewProjection(Scene* pScene);
	HRESULT SetViewTransform(Scene* pScene);

	Mat4x4 GetProjection() { return m_projection; }
	Mat4x4 GetView() { return m_view; }

	void SetCameraOffset(const Vec4& cameraOffset)
	{
		m_camOffsetVector = cameraOffset;
	}

protected:
	Frustum m_frustum;
	Mat4x4 m_projection;
	Mat4x4 m_view;
	bool m_bActive;
	bool m_bDebugCamera;
	SceneNode* m_pFollowTarget;
	SceneNode* m_pViewTarget;
	Vec4 m_camOffsetVector;
};