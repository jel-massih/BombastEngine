#include "GraphicsManager.h"

#include "../Bombast/BombastApp.h"

GraphicsManager::GraphicsManager()
{
	m_pRenderer = 0;
	m_pColorShader = 0;
	m_pTextureShader = 0;
}

GraphicsManager::GraphicsManager(const GraphicsManager& other)
{

}

GraphicsManager::~GraphicsManager()
{

}

bool GraphicsManager::Initialize(HWND hwnd)
{
	bool result;
	
	Renderer renderImpl = GetRendererImpl();
	if (renderImpl == Renderer_D3D11)
	{
		m_pRenderer = BE_NEW D3DClass11;
	}
	
	if (!m_pRenderer)
	{
		BE_ERROR("Failed to create Renderer");
		return false;
	}

	if (!m_pRenderer->VInitialize(g_pApp->m_options.m_screenSize.x, g_pApp->m_options.m_screenSize.y, true, hwnd, g_pApp->m_options.m_bFullscreen, 1000.0f, 0.1f))
	{
		BE_ERROR("Could not initialize Renderer");
		return FALSE;
	}

	m_pColorShader = BE_NEW ColorShaderClass;
	if (!m_pColorShader)
	{
		return false;
	}

	result = m_pColorShader->Initialize(m_pRenderer->GetDevice());
	if (!result)
	{
		BE_ERROR("Could not initialize the ColorShader Object!");
		return FALSE;
	}

	m_pTextureShader = BE_NEW TextureShaderClass;
	if (!m_pTextureShader)
	{
		return false;
	}

	result = m_pTextureShader->Initialize(m_pRenderer->GetDevice());
	if (!result)
	{
		BE_ERROR("Could not initialize the TextureShader Object!");
		return FALSE;
	}

	return true;
}

void GraphicsManager::Shutdown()
{
	if (m_pTextureShader)
	{
		m_pTextureShader->Shutdown();

		SAFE_DELETE(m_pTextureShader);
	}

	if (m_pColorShader)
	{
		m_pColorShader->Shutdown();

		SAFE_DELETE(m_pColorShader);
	}

	if (m_pRenderer)
	{
		m_pRenderer->VShutdown();

		SAFE_DELETE(m_pRenderer);
	}

	return;
}

IRenderer* GraphicsManager::GetRenderer()
{
	return m_pRenderer;
}

GraphicsManager::Renderer GraphicsManager::GetRendererImpl()
{
	return Renderer_D3D11;
}
