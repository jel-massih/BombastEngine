#include "GraphicsManager.h"

#include "../Bombast/BombastApp.h"

GraphicsManager::GraphicsManager()
{
	m_pD3D = 0;
	m_pCamera = 0;
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

	m_pD3D = BE_NEW D3DClass;
	if (!m_pD3D)
	{
		return false;
	}

	if (!m_pD3D->Initialize(BombastApp::GetGameInstance()->m_options.m_screenSize.x, BombastApp::GetGameInstance()->m_options.m_screenSize.y, true, hwnd, false, 1000.0f, 0.1f))
	{
		BE_ERROR(L"Could not initialize D3D11");
		return FALSE;
	}

	m_pCamera = BE_NEW CameraClass;
	if (!m_pCamera)
	{
		return false;
	}

	m_pCamera->SetPosition(0.0f, 0.0f, -10.0f);

	m_pColorShader = new ColorShaderClass;
	if (!m_pColorShader)
	{
		return false;
	}

	result = m_pColorShader->Initialize(m_pD3D->GetDevice());
	if (!result)
	{
		BE_ERROR(L"Could not initialize the ColorShader Object!")
		return FALSE;
	}

	m_pTextureShader = new TextureShaderClass;
	if (!m_pTextureShader)
	{
		return false;
	}

	result = m_pTextureShader->Initialize(m_pD3D->GetDevice());
	if (!result)
	{
		BE_ERROR(L"Could not initialize the TextureShader Object!")
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

	SAFE_DELETE(m_pCamera);

	if (m_pD3D)
	{
		m_pD3D->Shutdown();

		SAFE_DELETE(m_pD3D);
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

	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_pD3D->BeginScene(0.2f, 0.2f, 0.5f, 1.0f);

	m_pCamera->Render();

	m_pCamera->GetViewMatrix(viewMatrix);
	m_pD3D->GetWorldMatrix(worldMatrix);
	m_pD3D->GetProjectionMatrix(projectionMatrix);
	m_pD3D->GetOrthoMatrix(orthoMatrix);
	
	m_pD3D->EnableZBuffer(false);
	
	//Get Bitmaps Vector and render each one
	std::vector<BitmapClass*> bitmaps = BombastApp::GetGameInstance()->GetEntitiesManager()->GetBitmaps();
	for (BitmapClass* bitmap : bitmaps)
	{
		//prepare bitmap vertex and index buffers for drawing
		result = bitmap->Render(m_pD3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		//Render Bitmap with texture shader
		result = m_pTextureShader->Render(m_pD3D->GetDeviceContext(), bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, bitmap->GetTexture());
		if (!result)
		{
			return false;
		}
	}
	
	m_pD3D->EnableZBuffer(true);

	m_pD3D->EndScene();

	return true;
}

D3DClass* GraphicsManager::GetD3DClass()
{
	return m_pD3D;
}