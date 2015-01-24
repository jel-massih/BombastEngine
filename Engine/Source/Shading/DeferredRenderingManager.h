#pragma once
#include "../Utilities/DirectXUtils.h"

class DeferredRenderingManager
{
public:
	DeferredRenderingManager();
	~DeferredRenderingManager();

	bool Initialize(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath, int texWidth, int texHeight);

	void StartRender(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV) const;
	void DrawRenderable(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix) const;
	void FinishRender();

private:

	ID3D11InputLayout* m_pGbufferInputLayout;
	ID3D11VertexShader* m_pFillGBufferVertexShader;
	ID3D11PixelShader* m_pFillGBufferPixelShader;

	SimpleTexture m_texGBuffer;
	SimpleTexture m_texGBuffer2;
	SimpleTexture m_texGBufResolved2;
	SimpleTexture m_texCoverageMask;

	ID3D11SamplerState* m_pTextureSampler;
	ID3D11SamplerState* m_pPointSampler;
	ID3D11DepthStencilState* m_pWriteStencilState;
	ID3D11DepthStencilState* m_pTestStencilState;

	ID3D11Buffer* m_pMatrixBuffer;

private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
};