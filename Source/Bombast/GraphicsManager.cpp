#include "GraphicsManager.h"

#include "../Bombast/BombastApp.h"

GraphicsManager::GraphicsManager()
{
	m_pD3D = 0;
	m_pCamera = 0;
	m_pModel = 0;
	m_pBitmap = 0;
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

	if (!m_pD3D->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, true, hwnd, false, 1000.0f, 0.1f))
	{
		BE_ERROR(L"Could not initialize D3D11");
		return FALSE;
	}

	m_pCamera = new CameraClass;
	if (!m_pCamera)
	{
		return false;
	}

	m_pCamera->SetPosition(0.0f, 0.0f, -10.0f);

	m_pModel = new ModelClass;
	if (!m_pModel)
	{
		return false;
	}

	result = m_pModel->Initialize(m_pD3D->GetDevice(), L"../../Game/Data/dirt01.dds");
	if (!result)
	{
		BE_ERROR(L"Could not initialize the Model Object");
		return false;
	}

	m_pBitmap = BE_NEW BitmapClass;
	if (!m_pBitmap)
	{
		return false;
	}

	result = m_pBitmap->Initialize(m_pD3D->GetDevice(), SCREEN_WIDTH, SCREEN_HEIGHT, L"../../Game/Data/dirt01.dds", 256, 256);
	if (!result)
	{
		BE_ERROR(L"Could not initialize the Bitmap Object!")
		return FALSE;
	}

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

	if (m_pBitmap)
	{
		m_pBitmap->Shutdown();
		
		SAFE_DELETE(m_pBitmap);
	}

	if (m_pModel)
	{
		m_pModel->Shutdown();

		SAFE_DELETE(m_pModel);
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

	/*m_pModel->Render(m_pD3D->GetDeviceContext());

	result = m_pTextureShader->Render(m_pD3D->GetDeviceContext(), m_pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_pModel->GetTexture());
	if (!result)
	{
		return false;
	}
	*/
	m_pD3D->EnableZBuffer(false);
	
	//prepare bitmap vertex and index buffers for drawing
	result = m_pBitmap->Render(m_pD3D->GetDeviceContext(), 0, 0);
	if (!result) 
	{
		return false;
	}

	//Render Bitmap with texture shader
	result = m_pTextureShader->Render(m_pD3D->GetDeviceContext(), m_pBitmap->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_pBitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	m_pD3D->EnableZBuffer(true);

	m_pD3D->EndScene();

	return true;
}