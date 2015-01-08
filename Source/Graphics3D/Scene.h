#pragma once

#include "Geometry.h"
#include "SceneNode.h"
#include "../Events/EventManager.h"

typedef std::map<ActorId, ISceneNode*> SceneActorMap;

class CameraNode;
class SkyNode;
class LightNode;
class LightManager;
class D3D11GridNode;

class Scene
{
protected:
	SceneNode* m_pRoot;
	CameraNode* m_pCamera;
	D3D11GridNode* m_pGrid;
	IRenderer* m_pRenderer;

	BMMatrixStack* m_pMatrixStack;
	SceneActorMap m_actorMap;

	void RenderAlphaPass();

public:
	Scene(IRenderer* renderer);
	virtual ~Scene();

	HRESULT OnRender();
	HRESULT OnRestore();
	HRESULT OnLostDevice();
	HRESULT OnUpdate(const int deltaMS);
	ISceneNode* FindActor(ActorId id);
	bool AddChild(ActorId id, ISceneNode* kid);
	bool RemoveChild(ActorId id);

	void NewRenderComponentDelegate(IEventDataPtr pEventData);

	void SetCamera(CameraNode* camera) { m_pCamera = camera; }
	const CameraNode* GetCamera() const { return m_pCamera; }

	//p.541
	const Mat4x4 GetTopMatrix()
	{
		return static_cast<const Mat4x4>(*m_pMatrixStack->Top());
	}
	
	void PushAndSetMatrix(const Mat4x4 &toWorld) {
		m_pMatrixStack->Push();
		m_pMatrixStack->MultMatrixLocal(&toWorld);
		Mat4x4 mat = GetTopMatrix();
		m_pRenderer->VSetWorldTransform(&mat);
	}

	void PopMatrix()
	{
		m_pMatrixStack->Pop();
		Mat4x4 mat = GetTopMatrix();
		m_pRenderer->VSetWorldTransform(&mat);
	}

	IRenderer* GetRenderer() { return m_pRenderer; }

	HRESULT Pick(RayCast *pRayCast) { return m_pRoot->VPick(this, pRayCast); }
};