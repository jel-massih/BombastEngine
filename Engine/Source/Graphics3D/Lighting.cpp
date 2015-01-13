#include "Lighting.h"
#include "../Actor/RenderComponent.h"

void LightProperties::GetLightTypeFromString(std::string type)
{
	m_lightType = BLT_INVALID;

	if (type == "directional")
	{
		m_lightType = BLT_DIRECTIONAL;
	}
	if (type == "point")
	{
		m_lightType = BLT_POINT;
	}
	if (type == "spot")
	{
		m_lightType = BLT_SPOT;
	}
	if (type == "area")
	{
		m_lightType = BLT_AREA;
	}
}

LightNode::LightNode(const ActorId actorId, BaseRenderComponent* renderComponent, const LightProperties& props, const Mat4x4* t)
	: SceneNode(actorId, renderComponent, RenderPass_NotRendered, t)
{
	m_lightProperties = props;
}

HRESULT D3DLightNode11::VOnUpdate(Scene* pScene, const float deltaMs)
{
	LightRenderComponent* lrc = static_cast<LightRenderComponent*>(m_pRenderComponent);
	m_properties.GetMaterial().SetDiffuse(lrc->GetColor());
	
	return S_OK;
}

void LightingManager::CalcLighting(Scene* pScene)
{
	pScene->GetRenderer()->VCalcLighting(&m_lights, MAXIMUM_LIGHTS_SUPPORTED);

	int count = 0;
	BE_ASSERT(m_lights.size() < MAXIMUM_LIGHTS_SUPPORTED);
	for (auto light = m_lights.begin(); light != m_lights.end(); light++)
	{
		if (count == 0)
		{
			Vec4 ambient = (*light)->VGet()->GetMaterial().GetAmbient();
			m_lightAmbient = Vec4(ambient.x, ambient.y, ambient.z, 1.0f);
		}

		Vec3 lightDir = (*light)->GetDirection();
		m_lightDir[count] = Vec4(lightDir.x, lightDir.y, lightDir.z, 1.0f);
		m_lightDiffuse[count] = (*light)->VGet()->GetMaterial().GetDiffuse();
		Vec4 specular;
		float power;
		(*light)->VGet()->GetMaterial().GetSpecular(specular, power);
		m_lightSpecular[count] = Vec4(specular.x, specular.y, specular.z, specular.w);
		m_lightSpecularPower[count] = power;
	}
}

void LightingManager::CalcLighting(ConstantBuffer_Lighting* pLighting, SceneNode* pNode)
{
	int count = GetLightCount();
	if (count)
	{
		pLighting->m_lightAmbient = *GetAmbientLight();
		memcpy(pLighting->m_lightDir, GetLightDirection(), sizeof(Vec4) * count);
		memcpy(pLighting->m_lightDiffuse, GetLightDiffuse(), sizeof(Vec4) * count);
		pLighting->m_numLights = count;
	}
}