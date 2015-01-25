#pragma once
#include "../Utilities/DirectXUtils.h"

class DeferredRenderingManager
{
public:
	DeferredRenderingManager();
	~DeferredRenderingManager();

	bool Initialize(ID3D11Device* device, int texWidth, int texHeight);

	void StartRender(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV) const;
	void DrawRenderable(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix) const;
	void FinishRender(ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV) const;

private:
	bool InitializeGBufferShader(ID3D11Device* device);
	bool InitializeLightShader(ID3D11Device* device);

private:

	ID3D11InputLayout* m_pGbufferInputLayout;
	ID3D11VertexShader* m_pFillGBufferVertexShader;
	ID3D11PixelShader* m_pFillGBufferPixelShader;

	ID3D11InputLayout* m_pLightingInputLayout;
	ID3D11VertexShader* m_pLightingVertexShader;
	ID3D11PixelShader* m_pLightingPixelShader;

	SimpleTexture m_texGBuffer;
	SimpleTexture m_texGBuffer2;

	ID3D11SamplerState* m_pTextureSampler;
	ID3D11SamplerState* m_pLightPointSampler;

	ID3D11Buffer* m_pMatrixBuffer;

private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
};