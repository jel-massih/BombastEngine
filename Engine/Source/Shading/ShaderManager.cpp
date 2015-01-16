#include "ShaderManager.h"

ShaderManager::~ShaderManager()
{
	if (m_pLightShader)
	{
		m_pLightShader->Shutdown();

		SAFE_DELETE(m_pLightShader);
	}

	if (m_pColorShader)
	{
		m_pColorShader->Shutdown();

		SAFE_DELETE(m_pColorShader);
	}

	if (m_pTextureShader)
	{
		m_pTextureShader->Shutdown();

		SAFE_DELETE(m_pTextureShader);
	}

	if (m_pMultiTextureShader)
	{
		m_pMultiTextureShader->Shutdown();

		SAFE_DELETE(m_pMultiTextureShader);
	}
}

bool ShaderManager::Initialize(IRenderer* renderer)
{
	bool result;

	m_pLightShader = BE_NEW LightShader;
	if (!m_pLightShader)
	{
		BE_ERROR("Could not Allocate the LightShader Object!");
	}

	result = m_pLightShader->Initialize(renderer->GetDevice());
	if (!result)
	{
		BE_ERROR("Could not initialize the LightShader Object!");
		return FALSE;
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
		return FALSE;
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
		return FALSE;
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
		return FALSE;
	}
}

bool ShaderManager::RenderRenderable(SceneNode* pRenderableNode, int indexCount, const Scene* pScene) const
{
	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
	ID3D11DeviceContext* context = pRenderer->GetDeviceContext();

	Mat4x4 worldMatrix, viewMatrix, projectionMatrix;
	pRenderer->VGetViewMatrix(viewMatrix);
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetProjectionMatrix(projectionMatrix);

	switch (pRenderableNode->VGet()->GetMaterial().GetShaderType())
	{
	case BSHADER_TYPE_COLOR:
		return m_pColorShader->Render(context, indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), pRenderableNode->VGet()->GetMaterial().GetDiffuse());
		break;
	case BSHADER_TYPE_LIT:
		return m_pLightShader->Render(context, indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), &pRenderableNode->VGet()->GetMaterial(), pScene);
		break;
	case BSHADER_TYPE_TEXTURE:
		if (pRenderableNode->VGet()->GetMaterial().GetTextures().size() <= 0)
		{
			return false;
		}
		return m_pTextureShader->Render(context, indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), pRenderableNode->VGet()->GetMaterial().GetTextures().front()->GetTexture());
		break;
	case BSHADER_TYPE_MULTI_TEXTURE:
		return m_pMultiTextureShader->Render(context, indexCount, XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix), pRenderableNode->VGet()->GetMaterial().GetTextures());
		break;
	}
}