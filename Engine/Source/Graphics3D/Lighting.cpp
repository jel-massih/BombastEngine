#include "Lighting.h"
#include "../Actor/RenderComponent.h"

LightNode::LightNode(const ActorId actorId, BaseRenderComponent* renderComponent, const LightProperties& props, const Mat4x4* t)
	: SceneNode(actorId, renderComponent, RenderPass_NotRendered, t)
{
	m_lightProperties = props;
}

HRESULT D3DLightNode11::VOnUpdate(Scene* pScene, DWORD const elapsedMs)
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
			Color ambient = (*light)->VGet()->GetMaterial().GetAmbient();
			m_lightAmbient = Vec4(ambient.r, ambient.g, ambient.b, 1.0f);
		}

		Vec3 lightDir = (*light)->GetDirection();
		m_lightDir[count] = Vec4(lightDir.x, lightDir.y, lightDir.z, 1.0f);
		m_lightDiffuse[count] = (*light)->VGet()->GetMaterial().GetDiffuse();
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