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
	Vec4 m_diffuseColor;
	Vec4 m_specularColor;
	float m_specularPower;

	static LightType GetLightTypeFromString(const std::string type);
	static std::string GetLightTypeNameFromLightType(const LightType type);
};

class LightNode : public SceneNode
{
public:
	LightNode(const ActorId actorId, BaseRenderComponent* renderComponent, const LightProperties& props, const Mat4x4* t);
	const LightProperties* GetLightProperties() const { return &m_lightProperties; }

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
	LightingManager() { m_lightAmbient = Vec4(0.15f, 0.15f, 0.15f, 0.15f); }

	void CalcLighting(Scene* pScene);
	void CalcLighting(ConstantBuffer_Lighting* pLighting, SceneNode* pNode);
	size_t GetLightCount() const { return m_lights.size(); }
	void SetGlobalAmbient(const Vec4 newAmbient) { m_lightAmbient = newAmbient; }
	const Vec4* GetGlobalAmbient() const { return &m_lightAmbient; }
	const Vec4* GetLightDirection() const { return m_lightDir; }
	const Vec4* GetLightDiffuse() const { return m_lightDiffuse; }
	const Vec4* GetLightSpecular() const { return m_lightSpecular; }
	const float* GetLightSpecularPower() const { return m_lightSpecularPower; }
	
	std::list<LightNode*> m_lights;
protected:
	Vec4 m_lightDir[MAX_LIGHTS_SUPPORTED];
	Vec4 m_lightDiffuse[MAX_LIGHTS_SUPPORTED];
	Vec4 m_lightSpecular[MAX_LIGHTS_SUPPORTED];
	float m_lightSpecularPower[MAX_LIGHTS_SUPPORTED];
	Vec4 m_lightAmbient;
};