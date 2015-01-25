#include "ShaderManager.h"

ShaderManager::~ShaderManager()
{
	SAFE_DELETE(m_pLightShader);
	SAFE_DELETE(m_pColorShader);
	SAFE_DELETE(m_pTextureShader);
	SAFE_DELETE(m_pMultiTextureShader);
	SAFE_DELETE(m_pLightmapShader);
	SAFE_DELETE(m_pDeferredLightShader);
}

bool ShaderManager::Initialize(IRenderer* renderer)
{
	bool result;

	m_pLightShader = BE_NEW LightShader;
	if (!m_pLightShader)
	{
		BE_ERROR("Could not Allocate the LightShader Object!");
		return false;
	}

	result = m_pLightShader->Initialize(renderer->GetDevice());
	if (!result)
	{
		BE_ERROR("Could not initialize the LightShader Object!");
		return false;
	}

	m_pColorShader = BE_NEW ColorShader;
	if (!m_pColorShader)
	{
		BE_ERROR("Could not Allocate the ColorShader Object!");
		return false;
	}

	result = m_pColorShader->Initialize(renderer->GetDevice());
	if (!result)
	{
		BE_ERROR("Could not initialize the ColorShader Object!");
		return false;
	}

	m_pTextureShader = BE_NEW TextureShader;
	if (!m_pTextureShader)
	{
		BE_ERROR("Could not Allocate the TextureShader Object!");
		return false;
	}

	result = m_pTextureShader->Initialize(renderer->GetDevice());
	if (!result)
	{
		BE_ERROR("Could not initialize the TextureShader Object!");
		return false;
	}

	m_pMultiTextureShader = BE_NEW MultiTextureShader;
	if (!m_pMultiTextureShader)
	{
		BE_ERROR("Could not Allocate the MultiTextureShader Object!");
		return false;
	}

	result = m_pMultiTextureShader->Initialize(renderer->GetDevice());
	if (!result)
	{
		BE_ERROR("Could not initialize the MultiTextureShader Object!");
		return false;
	}

	m_pLightmapShader = BE_NEW LightmapShader;
	if (!m_pLightmapShader)
	{
		BE_ERROR("Could not Allocate the LightmapShader Object!");
		return false;
	}

	result = m_pLightmapShader->Initialize(renderer->GetDevice());
	if (!result)
	{
		BE_ERROR("Could not initialize the LightmapShader Object!");
		return false;
	}

	m_pDeferredLightShader = BE_NEW DeferredLightShader;
	if (!m_pDeferredLightShader)
	{
		BE_ERROR("Could not Allocate the DeferredLightShader Object!");
		return false;
	}

	result = m_pDeferredLightShader->Initialize(renderer->GetDevice());
	if (!result)
	{
		BE_ERROR("Could not initialize the DeferredLightShader Object!");
		return false;
	}

	
	return true;
}

bool ShaderManager::RenderRenderable(SceneNode* pRenderableNode, Material* pMaterial, int indexCount, const Scene* pScene) const
{
	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
	ID3D11DeviceContext* context = pRenderer->GetDeviceContext();

	Mat4x4 worldMatrix, viewMatrix, projectionMatrix;
	pRenderer->VGetViewMatrix(viewMatrix);
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetProjectionMatrix(projectionMatrix);

	switch (pMaterial->GetShaderType())
	{
	case BSHADER_TYPE_COLOR:
		return m_pColorShader->Render(context, indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), pMaterial->GetDiffuse());
		break;
	case BSHADER_TYPE_LIT:
		return m_pLightShader->Render(context, indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), pMaterial, pScene);
		break;
	case BSHADER_TYPE_TEXTURE:
		if (pMaterial->GetTextures().size() <= 0)
		{
			return false;
		}
		return m_pTextureShader->Render(context, indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), pMaterial->GetTextures().front()->GetTexture());
		break;
	case BSHADER_TYPE_MULTI_TEXTURE:
		return m_pMultiTextureShader->Render(context, indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), pMaterial->GetTextures());
		break;
	case BSHADER_TYPE_LIGHTMAP:
		return m_pLightmapShader->Render(context, indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), pMaterial->GetTextures());
		break;
	case BSHADER_TYPE_DEFERRED_LIT:
		return m_pDeferredLightShader->Render(context, indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), g_pApp->GetGraphicsManager()->GetDeferredRenderingManager()->GetColorRenderTexture().GetShaderResourceView(), g_pApp->GetGraphicsManager()->GetDeferredRenderingManager()->GetNormalRenderTexture().GetShaderResourceView(), pScene);
		break;
	}

	return false;
}