//p. 508

#include "Shaders.h"
#include "SceneNode.h"

BombastEngine_Hlsl_VertexShader::BombastEngine_Hlsl_VertexShader()
{
	m_pVertexLayout11 = NULL;
	m_pVertexShader = NULL;
	m_pcbVSMatrices = NULL;
	m_pcbVSLighting = NULL;
	m_pcbVSMaterial = NULL;
	m_bEnableLights = true;
}

BombastEngine_Hlsl_VertexShader::~BombastEngine_Hlsl_VertexShader()
{
	SAFE_RELEASE(m_pVertexLayout11);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pcbVSMatrices);
	SAFE_RELEASE(m_pcbVSLighting);
	SAFE_RELEASE(m_pcbVSMaterial);
}

HRESULT BombastEngine_Hlsl_VertexShader::OnRestore(Scene* pScene)
{
	HRESULT hr = S_OK;

	SAFE_RELEASE(m_pVertexLayout11);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pcbVSMatrices);
	SAFE_RELEASE(m_pcbVSLighting);
	SAFE_RELEASE(m_pcbVSMaterial);

	return hr;
}