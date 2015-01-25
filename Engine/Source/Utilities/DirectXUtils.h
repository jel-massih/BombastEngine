#pragma once

inline HRESULT InitTexture2D(ID3D11Device* device, DXGI_FORMAT format, UINT width, UINT height, ID3D11Texture2D** ppTex2D, ID3D11ShaderResourceView** ppSRV, ID3D11RenderTargetView** ppRTV)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	hr = device->CreateTexture2D(&desc, NULL, ppTex2D);
	if (FAILED(hr)) {
		return hr;
	}

	if (ppSRV)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(*ppTex2D, &SRVDesc, ppSRV);
		if (FAILED(hr)) {
			return hr;
		}
	}

	if (ppRTV)
	{
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		RTVDesc.Format = desc.Format;
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		hr = device->CreateRenderTargetView(*ppTex2D, &RTVDesc, ppRTV);
		if (FAILED(hr)) {
			return hr;
		}
	}

	return hr;
}

class SimpleTexture
{
public:
	SimpleTexture() : m_pTexture2D(nullptr), m_pShaderResourceView(nullptr), m_pRenderTargetView(nullptr) {}

	HRESULT Initialize(ID3D11Device* device, DXGI_FORMAT format, UINT width, UINT height, bool bShaderResourceView = true, bool bRenderTargetView = true)
	{
		HRESULT hr = S_OK;
		Release();

		hr = InitTexture2D(device, format, width, height, &this->m_pTexture2D, bShaderResourceView ? &this->m_pShaderResourceView : NULL, bRenderTargetView ? &this->m_pRenderTargetView : NULL);
		
		return hr;
	}

	ID3D11Texture2D* GetTexture() const { return m_pTexture2D; }
	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pShaderResourceView; }
	ID3D11RenderTargetView* GetRenderTargetView() const { return m_pRenderTargetView; }

	void Release()
	{
		SAFE_RELEASE(m_pTexture2D);
		SAFE_RELEASE(m_pShaderResourceView);
		SAFE_RELEASE(m_pRenderTargetView);
	}

private:
	ID3D11Texture2D* m_pTexture2D;
	ID3D11ShaderResourceView* m_pShaderResourceView;
	ID3D11RenderTargetView* m_pRenderTargetView;
};