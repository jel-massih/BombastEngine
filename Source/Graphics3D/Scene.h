#ifndef SCENE_H
#define SCENE_H

//Scene.h - Container class for 3d graphics scenes

#include "Geometry.h"
#include "SceneNode.h"

//STL map allowing lookup of scenenode by actorid
typedef std::map<ActorId, ISceneNode*> SceneActorMap;

//p.536
class CameraNode;
class SkyNode;
class LightNode;
class LightManager;

class Scene
{
protected:
	SceneNode* m_pRoot;
	CameraNode* m_pCamera;
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
};

#endif