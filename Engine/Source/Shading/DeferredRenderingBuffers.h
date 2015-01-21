#pragma once

const int BUFFER_COUNT = 3;

class DeferredRenderingBuffers
{
public:
	DeferredRenderingBuffers();
	~DeferredRenderingBuffers();

	bool Initialize(ID3D11Device* device, int texWidth, int texHeight);

	void SetRenderTargets(ID3D11DeviceContext* context);
	void ClearRenderTargets(ID3D11DeviceContext* context, Vec4 clearColor);

	ID3D11ShaderResourceView* GetShaderResourceView(int index) { return m_shaderResources[index];
	}

private:
	int m_textureWidth, m_textureHeight;

	ID3D11Texture2D* m_renderTargetTextures[BUFFER_COUNT];
	ID3D11RenderTargetView* m_renderTargetViews[BUFFER_COUNT];
	ID3D11ShaderResourceView* m_shaderResources[BUFFER_COUNT];
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D11_VIEWPORT m_viewport;
};