#pragma once

struct LightProperties
{
	float m_range;
	float m_falloff;
};

class LightNode : public SceneNode
{
public:
	LightNode(const ActorId actorId, BaseRenderComponent* renderComponent, const LightProperties& props, const Mat4x4* t);

protected:
	LightProperties m_lightProperties;
};

class D3DLightNode11 : public LightNode
{
public:
	D3DLightNode11(const ActorId actorId, BaseRenderComponent* renderComponent, const LightProperties& props, const Mat4x4* t)
		: LightNode(actorId, renderComponent, props, t){}

	virtual HRESULT VOnRestore() { return S_OK; }
	virtual HRESULT VOnUpdate(Scene* pScene, DWORD const elapsedMs);
};

class LightingManager
{
public:
	void CalcLighting(Scene* pScene);
	void CalcLighting(ConstantBuffer_Lighting* pLighting, SceneNode* pNode);
	int GetLightCount() const { return m_lights.size(); }
	const Vec4* GetAmbientLight() const { return &m_lightAmbient; }
	const Vec4* GetLightDirection() const { return m_lightDir; }
	const Color* GetLightDiffuse() const { return m_lightDiffuse; }
	
	std::list<LightNode*> m_lights;
protected:
	Vec4 m_lightDir[MAXIMUM_LIGHTS_SUPPORTED];
	Color m_lightDiffuse[MAXIMUM_LIGHTS_SUPPORTED];
	Vec4 m_lightAmbient;
};