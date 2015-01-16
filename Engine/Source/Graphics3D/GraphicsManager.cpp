#include "GraphicsManager.h"

#include "../Bombast/BombastApp.h"

GraphicsManager::GraphicsManager()
{
	m_pRenderer = nullptr;
	m_pShaderManager = nullptr;
}

GraphicsManager::~GraphicsManager()
{
	if (m_pRenderer)
	{
		m_pRenderer->VShutdown();

		SAFE_DELETE(m_pRenderer);
	}

	SAFE_DELETE(m_pShaderManager);
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

	if (!m_pRenderer->VInitialize(g_pApp->m_options.m_screenSize.x, g_pApp->m_options.m_screenSize.y, g_pApp->m_options.m_bVsync, hwnd, g_pApp->m_options.m_bFullscreen, 1000.0f, 0.1f))
	{
		BE_ERROR("Could not initialize Renderer");
		return FALSE;
	}

	m_pShaderManager = BE_NEW ShaderManager;
	if (!m_pShaderManager)
	{
		BE_ERROR("Could not Allocate the ShaderManager Object!");
		return false;
	}

	result = m_pShaderManager->Initialize(m_pRenderer);
	if (!result)
	{
		BE_ERROR("Could not initialize the ShaderManager Object!");
		return FALSE;
	}


	return true;
}

IRenderer* GraphicsManager::GetRenderer()
{
	return m_pRenderer;
}

GraphicsManager::Renderer GraphicsManager::GetRendererImpl()
{
	return Renderer_D3D11;
}
