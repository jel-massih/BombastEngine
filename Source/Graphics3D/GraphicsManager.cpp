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
		BE_ERROR("Failed to create Renderer")
		return false;
	}

	if (!m_pRenderer->VInitialize(g_pApp->m_options.m_screenSize.x, g_pApp->m_options.m_screenSize.y, true, hwnd, false, 1000.0f, 0.1f))
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
		BE_ERROR("Could not initialize the ColorShader Object!")
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
		BE_ERROR("Could not initialize the TextureShader Object!")
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

bool GraphicsManager::Frame()
{
	bool result;
	
	//render graphics scene
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsManager::Render()
{
	bool result;

	Mat4x4 worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_pRenderer->VSetBackgroundColor(0.2f, 0.2f, 0.5f, 1.0f);
	m_pRenderer->VBeginScene();

	m_pRenderer->VGetViewMatrix(viewMatrix);
	m_pRenderer->VGetWorldMatrix(worldMatrix);
	m_pRenderer->VGetProjectionMatrix(projectionMatrix);
	m_pRenderer->VGetOrthoMatrix(orthoMatrix);
	
	m_pRenderer->VEnableZBuffer(false);
	
	//Get Bitmaps Vector and render each one
	std::vector<BitmapClass*> bitmaps = g_pApp->GetEntitiesManager()->GetBitmaps();
	for (BitmapClass* bitmap : bitmaps)
	{
		//prepare bitmap vertex and index buffers for drawing
		result = bitmap->Render(m_pRenderer->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		//Render Bitmap with texture shader
		result = m_pTextureShader->Render(m_pRenderer->GetDeviceContext(), bitmap->GetIndexCount(), DirectX::XMLoadFloat4x4(&worldMatrix), DirectX::XMLoadFloat4x4(&viewMatrix), DirectX::XMLoadFloat4x4(&orthoMatrix), bitmap->GetTexture());
		if (!result)
		{
			return false;
		}
	}
	
	m_pRenderer->VEnableZBuffer(true);
	m_pRenderer->VEndScene();

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
