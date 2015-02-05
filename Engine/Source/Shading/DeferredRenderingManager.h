#pragma once
#include "../Utilities/DirectXUtils.h"

class PostProcessRenderWindow;
class DeferredLightShader;

class DeferredRenderingManager
{
public:
	DeferredRenderingManager();
	~DeferredRenderingManager();

	bool Initialize(ID3D11Device* device, int texWidth, int texHeight);

	void StartRender(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV) const;
	void DrawRenderable(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* diffuseTexture, ID3D11ShaderResourceView* normalTexture) const;
	
	void DrawLightPass(ID3D11DeviceContext* context, const Scene* pScene) const;

	const SimpleTexture& GetColorRenderTexture() const { return m_texGBuffer; }
	const SimpleTexture& GetNormalRenderTexture() const { return m_texGBuffer2; }

private:
	bool InitializeGBufferShader(ID3D11Device* device);

private:

	ID3D11InputLayout* m_pGbufferInputLayout;
	ID3D11VertexShader* m_pFillGBufferVertexShader;
	ID3D11PixelShader* m_pFillGBufferPixelShader;

	SimpleTexture m_texGBuffer;
	SimpleTexture m_texGBuffer2;

	ID3D11SamplerState* m_pTextureSampler;

	ID3D11Buffer* m_pMatrixBuffer;

	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;


	DeferredLightShader* m_pDeferredLightShader;
	PostProcessRenderWindow* m_pPostProcessRenderWindow;

private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
};