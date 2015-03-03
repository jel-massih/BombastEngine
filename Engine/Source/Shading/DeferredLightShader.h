#pragma once

class Material;

class DeferredLightShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMMATRIX inverseViewProjection;
		XMFLOAT3 lightDirection;
		XMFLOAT3 camPos;
		XMFLOAT2 padding;
	};

public:
	DeferredLightShader();
	~DeferredLightShader();

	bool Initialize(ID3D11Device* device);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* depthTexture, const Scene* pScene);

private:
	bool InitializeShader(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* depthTexture, const Scene* pScene);
	void RenderShader(ID3D11DeviceContext* deviceContext, int);

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pLightBuffer;

	ID3D11SamplerState* m_pPointSampleState;
	ID3D11SamplerState* m_pDepthSampleState;
};