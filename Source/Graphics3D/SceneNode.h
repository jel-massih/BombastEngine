#ifndef SCENE_NODE_H
#define SCENE_NODE_H

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

	bool HasAlpha() const { return m_material.HasAlpha(); }
	float GetAlpha() const { return m_material.GetAlpha(); }
	AlphaType GetAlphaType() const { return m_alphaType; }

	RenderPass GetRenderPass() const { return m_renderPass; }
	float GetRadius() const { return m_radius; }

	Material GetMaterial() const { return m_material; }

protected:
	ActorId m_actorId;
	std::string m_name;
	Mat4x4 m_toWorld, m_fromWorld;
	float m_radius;
	RenderPass m_renderPass;
	Material m_material;
	AlphaType m_alphaType;

	void SetAlpha(const float alpha) { m_alphaType = AlphaMaterial; m_material.SetAlpha(alpha); }
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
	virtual HRESULT VOnUpdate(Scene* pScene, DWORD const elapsedMs);

	virtual HRESULT VPreRender(Scene* pScene);
	virtual bool VIsVisible(Scene* pScene) const;
	virtual HRESULT VRender(Scene* pScene) { return S_OK; }
	virtual HRESULT VRenderChildren(Scene* pScene);
	virtual HRESULT VPostRender(Scene* pScene);

	virtual bool VAddChild(ISceneNode* kid);
	virtual bool VRemoveChild(ActorId id);
	virtual HRESULT VOnLostDevice(Scene* pScene);
	virtual HRESULT VPick(Scene* pScene, RayCast* pRayCast);

	void SetAlpha(float alpha);
	float GetAlpha() const { return m_properties.GetAlpha(); }

	Vec3 GetPosition() const { return m_properties.m_toWorld.GetPosition(); }
	void SetPosition(const Vec3& pos) { m_properties.m_toWorld.SetPosition(pos); }

	const Vec3 GetWorldPosition() const;

	Vec3 GetDirection() const { return m_properties.m_toWorld.GetDirection(); }

	void SetRadius(const float radius) { m_properties.m_radius = radius; }
	void SetMaterial(const Material& mat) { m_properties.m_material = mat; }

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
	virtual HRESULT VRenderChildren(Scene* pScene);
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
		m_pTarget(nullptr),
		m_camOffsetVector(0.0f, 0.0f, -10.0f, 0.0f)
	{
	}

	virtual HRESULT VRender(Scene* pScene);
	virtual HRESULT VOnRestore(Scene* pScene);
	virtual bool VIsVisible(Scene* pScene) const { return m_bActive; }

	const Frustum& GetFrustum() const { return m_frustum; }
	void SetTarget(SceneNode* pTarget) { m_pTarget = pTarget; }
	void ClearTarget() { m_pTarget = nullptr; }
	SceneNode* GetTarget() { return m_pTarget; }

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
	SceneNode* m_pTarget;
	Vec4 m_camOffsetVector;
};
#endif