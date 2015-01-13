#pragma once

enum LightType
{
	BLT_INVALID = -1,
	BLT_DIRECTIONAL,
	BLT_POINT,
	BLT_SPOT,
	BLT_AREA,
	BLT_MAX = BLT_AREA
};

struct LightProperties
{
	float m_range;
	float m_falloff;
	LightType m_lightType;

	void GetLightTypeFromString(const std::string type);
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
	virtual HRESULT VOnUpdate(Scene* pScene, const float deltaMs);
};

class LightingManager
{
public:
	void CalcLighting(Scene* pScene);
	void CalcLighting(ConstantBuffer_Lighting* pLighting, SceneNode* pNode);
	int GetLightCount() const { return m_lights.size(); }
	const Vec4* GetAmbientLight() const { return &m_lightAmbient; }
	const Vec4* GetLightDirection() const { return m_lightDir; }
	const Vec4* GetLightDiffuse() const { return m_lightDiffuse; }
	const Vec4* GetLightSpecular() const { return m_lightSpecular; }
	const float* GetLightSpecularPower() const { return m_lightSpecularPower; }
	
	std::list<LightNode*> m_lights;
protected:
	Vec4 m_lightDir[MAXIMUM_LIGHTS_SUPPORTED];
	Vec4 m_lightDiffuse[MAXIMUM_LIGHTS_SUPPORTED];
	Vec4 m_lightSpecular[MAXIMUM_LIGHTS_SUPPORTED];
	float m_lightSpecularPower[MAXIMUM_LIGHTS_SUPPORTED];
	Vec4 m_lightAmbient;
};