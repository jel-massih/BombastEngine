#include "DeferredRenderingBuffers.h"

DeferredRenderingBuffers::DeferredRenderingBuffers()
{
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		m_renderTargetTextures[i] = nullptr;
		m_shaderResources[i] = nullptr;
		m_renderTargetViews[i] = nullptr;
	}

	m_pDepthStencilBuffer = nullptr;
	m_pDepthStencilView = nullptr;
}

DeferredRenderingBuffers::~DeferredRenderingBuffers()
{
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilBuffer);

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		SAFE_RELEASE(m_shaderResources[i]);
		SAFE_RELEASE(m_renderTargetViews[i]);
		SAFE_RELEASE(m_renderTargetTextures[i]);
	}
}

bool DeferredRenderingBuffers::Initialize(ID3D11Device* device, int texWidth, int texHeight)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC texDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	m_textureWidth = texWidth;
	m_textureHeight = texHeight;

	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		result = device->CreateTexture2D(&texDesc, NULL, &m_renderTargetTextures[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	renderTargetViewDesc.Format = texDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		result = device->CreateRenderTargetView(m_renderTargetTextures[i], &renderTargetViewDesc, &m_renderTargetViews[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	shaderResourceViewDesc.Format = texDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		result = device->CreateShaderResourceView(m_renderTargetTextures[i], &shaderResourceViewDesc, &m_shaderResources[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = texWidth;
	depthBufferDesc.Height = texHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	m_viewport.Width = (float)texWidth;
	m_viewport.Height = (float)texHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	return true;
}

void DeferredRenderingBuffers::SetRenderTargets(ID3D11DeviceContext* context)
{
	context->OMSetRenderTargets(BUFFER_COUNT, m_renderTargetViews, m_pDepthStencilView);

	//context->RSSetViewports(1, &m_viewport);
}

void DeferredRenderingBuffers::ClearRenderTargets(ID3D11DeviceContext* context, Vec4 clearColor)
{
	float color[4];

	color[0] = clearColor.x;
	color[1] = clearColor.y;
	color[2] = clearColor.z;
	color[3] = clearColor.w;

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		context->ClearRenderTargetView(m_renderTargetViews[i], color);
	}

	context->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}