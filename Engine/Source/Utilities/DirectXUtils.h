#pragma once

#include "../Bombast/BombastApp.h"

inline HRESULT InitTexture2D(ID3D11Device* device, DXGI_FORMAT format, UINT width, UINT height, ID3D11Texture2D** ppTex2D, ID3D11ShaderResourceView** ppSRV, ID3D11RenderTargetView** ppRTV, ID3D11DepthStencilView** ppDSV)
{
	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.ArraySize = 1;
	if (ppRTV) desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	if (ppSRV) desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	if (ppDSV) desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	desc.Format = format;
	desc.Width = width;
	desc.Height = height;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.MipLevels = 1;
	BE_HRETURN(device->CreateTexture2D(&desc, NULL, ppTex2D), "Failed to create Texture 2D");

	DXGI_FORMAT srvFormat = desc.Format;
	DXGI_FORMAT rtvFormat = desc.Format;
	DXGI_FORMAT dsvFormat = desc.Format;

	if (format == DXGI_FORMAT_R24G8_TYPELESS)
	{
		srvFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		rtvFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		dsvFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	}

	if (ppSRV)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.Format = srvFormat;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels - 1;

		BE_HRETURN(device->CreateShaderResourceView(*ppTex2D, &SRVDesc, ppSRV), "Failed to create Shader Resource View");
	}

	if (ppRTV)
	{
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		ZeroMemory(&RTVDesc, rtvFormat);
		RTVDesc.Format = rtvFormat;
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		BE_HRETURN(device->CreateRenderTargetView(*ppTex2D, &RTVDesc, ppRTV), "Failed to create Render Target View");
	}

	if (ppDSV)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
		ZeroMemory(&DSVDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		DSVDesc.Format = dsvFormat;
		DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		BE_HRETURN(device->CreateDepthStencilView(*ppTex2D, &DSVDesc, ppDSV), "Failed to Create Despth Stencil View");
	}

	return hr;
}

class SimpleTexture
{
public:
	SimpleTexture() : m_pTexture2D(nullptr), m_pDepthStencilView(nullptr), m_pShaderResourceView(nullptr), m_pRenderTargetView(nullptr) {}

	HRESULT Initialize(ID3D11Device* device, DXGI_FORMAT format, UINT width, UINT height, bool bShaderResourceView = true, bool bRenderTargetView = true, bool bDepthStencilView = false)
	{
		HRESULT hr;
		Release();

		BE_HRETURN(InitTexture2D(device, format, width, height, &this->m_pTexture2D, bShaderResourceView ? &this->m_pShaderResourceView : NULL, bRenderTargetView ? &this->m_pRenderTargetView : NULL, bDepthStencilView ? &this->m_pDepthStencilView : NULL), "Failed to initialize Texture2D");
		
		return S_OK;
	}

	ID3D11Texture2D* GetTexture() const { return m_pTexture2D; }
	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pShaderResourceView; }
	ID3D11RenderTargetView* GetRenderTargetView() const { return m_pRenderTargetView; }
	ID3D11DepthStencilView* GetDepthStencilView() const { return m_pDepthStencilView; }

	void Release()
	{
		SAFE_RELEASE(m_pTexture2D);
		SAFE_RELEASE(m_pDepthStencilView);
		SAFE_RELEASE(m_pShaderResourceView);
		SAFE_RELEASE(m_pRenderTargetView);
	}

private:
	ID3D11Texture2D* m_pTexture2D;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11ShaderResourceView* m_pShaderResourceView;
	ID3D11RenderTargetView* m_pRenderTargetView;
};