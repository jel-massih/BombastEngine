#include "Lighting.h"
#include "../Actor/RenderComponent.h"

LightType LightProperties::GetLightTypeFromString(std::string type)
{
	LightType lightType = BLT_INVALID;

	if (type == "directional")
	{
		lightType = BLT_DIRECTIONAL;
	}
	if (type == "point")
	{
		lightType = BLT_POINT;
	}
	if (type == "spot")
	{
		lightType = BLT_SPOT;
	}
	if (type == "area")
	{
		lightType = BLT_AREA;
	}

	return lightType;
}

LightNode::LightNode(const ActorId actorId, BaseRenderComponent* renderComponent, const LightProperties& props, const Mat4x4* t)
	: SceneNode(actorId, renderComponent, RenderPass_NotRendered, t)
{
	m_lightProperties = props;
}

HRESULT D3DLightNode11::VOnUpdate(Scene* pScene, const float deltaMs)
{
	return S_OK;
}

void LightingManager::CalcLighting(Scene* pScene)
{
	pScene->GetRenderer()->VCalcLighting(&m_lights, MAX_LIGHTS_SUPPORTED);

	ZeroMemory(&m_lightDiffuse, sizeof(Vec4) * MAX_LIGHTS_SUPPORTED);
	ZeroMemory(&m_lightSpecular, sizeof(Vec4) * MAX_LIGHTS_SUPPORTED);
	ZeroMemory(&m_lightDir, sizeof(Vec4) * MAX_LIGHTS_SUPPORTED);
	ZeroMemory(&m_lightSpecularPower, sizeof(float) * MAX_LIGHTS_SUPPORTED);

	int count = 0;
	BE_ASSERT(m_lights.size() < MAX_LIGHTS_SUPPORTED);
	for (auto light = m_lights.begin(); light != m_lights.end(); light++)
	{
		Vec3 lightDir = (*light)->GetDirection();
		m_lightDir[count] = Vec4(lightDir.x, lightDir.y, lightDir.z, 1.0f);
		m_lightDiffuse[count] = (*light)->GetLightProperties()->m_diffuseColor;
		m_lightSpecular[count] = (*light)->GetLightProperties()->m_specularColor;
		m_lightSpecularPower[count] = (*light)->GetLightProperties()->m_specularPower;
	}
}

void LightingManager::CalcLighting(ConstantBuffer_Lighting* pLighting, SceneNode* pNode)
{
	size_t count = GetLightCount();
	if (count)
	{
		pLighting->m_lightAmbient = *GetGlobalAmbient();
		memcpy(pLighting->m_lightDir, GetLightDirection(), sizeof(Vec4) * count);
		memcpy(pLighting->m_lightDiffuse, GetLightDiffuse(), sizeof(Vec4) * count);
		pLighting->m_numLights = count;
	}
}