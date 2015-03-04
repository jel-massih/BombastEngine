#include "DeferredRenderingManager.h"
#include "../Graphics2D/PostProcessRenderWindow.h"
#include "DeferredLightShader.h"

DeferredRenderingManager::DeferredRenderingManager()
	: m_pDepthStencilBuffer(nullptr),
	m_pDepthStencilView(nullptr),
	m_pDeferredLightShader(nullptr),
	m_pPostProcessRenderWindow(nullptr),
	m_pDepthShaderResourceView(nullptr)
{
}

DeferredRenderingManager::~DeferredRenderingManager()
{
	SAFE_RELEASE(m_pDepthStencilBuffer);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthShaderResourceView);

	SAFE_DELETE(m_pDeferredLightShader);
	SAFE_DELETE(m_pPostProcessRenderWindow);

	m_texGBuffer.Release();
	m_texGBuffer2.Release();
}

bool DeferredRenderingManager::Initialize(ID3D11Device* device, int texWidth, int texHeight)
{
	HRESULT result;

	m_texGBuffer.Initialize(device, DXGI_FORMAT_R32G32B32A32_FLOAT, texWidth, texHeight, true, true);
	m_texGBuffer2.Initialize(device, DXGI_FORMAT_R32G32B32A32_FLOAT, texWidth, texHeight, true, true);

	{
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

		depthBufferDesc.Width = texWidth;
		depthBufferDesc.Height = texHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
		if (FAILED(result)) {
			BE_ERROR("Error: Failed to create Depth Buffer");
			return false;
		}
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		result = device->CreateDepthStencilView(m_pDepthStencilBuffer, &dsvDesc, &m_pDepthStencilView);
		if (FAILED(result)) {
			BE_ERROR("Error: Failed to create Depth Stencil View");
			return false;
		}
	}

	//Create Shader Resource
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		result = device->CreateShaderResourceView(m_pDepthStencilBuffer, &srvDesc, &m_pDepthShaderResourceView);
		if (FAILED(result)) {
			BE_ERROR("Error: Failed to create Depth Stencil Shader Resource View");
			return false;
		}
	}

	m_pDeferredLightShader = BE_NEW DeferredLightShader;
	if (!m_pDeferredLightShader)
	{
		BE_ERROR("Could not Allocate the DeferredLightShader Object!");
		return false;
	}

	result = m_pDeferredLightShader->Initialize(device);
	if (!result)
	{
		BE_ERROR("Could not initialize the DeferredLightShader Object!");
		return false;
	}

	m_pPostProcessRenderWindow = BE_NEW PostProcessRenderWindow;
	if (!m_pPostProcessRenderWindow)
	{
		BE_ERROR("Could not Allocate the PostProcessRenderWindow Object!");
		return false;
	}

	Point screenSize = g_pApp->GetScreenSize();
	result = m_pPostProcessRenderWindow->Initialize(device, screenSize.GetX(), screenSize.GetY());
	if (!result)
	{
		BE_ERROR("Could not initialize the PostProcessRenderWindow Object!");
		return false;
	}

	return true;
}

void DeferredRenderingManager::StartRender(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV, const float* clearBufferColor) const
{
	{
		//Fill G Buffer
		ID3D11RenderTargetView* pGBufRTV[] = { m_texGBuffer.GetRenderTargetView(), m_texGBuffer2.GetRenderTargetView() };
		context->OMSetRenderTargets(2, pGBufRTV, m_pDepthStencilView);

		context->ClearRenderTargetView(pGBufRTV[0], clearBufferColor);
		context->ClearRenderTargetView(pGBufRTV[1], clearBufferColor);
		context->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}

void DeferredRenderingManager::DrawLightPass(ID3D11DeviceContext* context, const Scene* pScene) const
{
	m_pPostProcessRenderWindow->Render(context);
	m_pDeferredLightShader->Render(context, m_pPostProcessRenderWindow->GetIndexCount(), m_texGBuffer.GetShaderResourceView(), m_texGBuffer2.GetShaderResourceView(), m_pDepthShaderResourceView, pScene);
}