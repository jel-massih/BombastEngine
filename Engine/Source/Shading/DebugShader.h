#pragma once

#include "../Resources/ResourceCache.h"

class DebugShader
{
public:
	DebugShader();
	~DebugShader();

	bool Initialize(ID3D11Device* device);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projMatrix, XMFLOAT4 pixelColor);

private:
	struct ConstantBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct PixelBufferType
	{
		XMFLOAT4 color;
	};

private:
	bool InitializeShader(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix,
		XMFLOAT4 pixelColor);
	void RenderShaders(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11SamplerState* m_pSampleState;
	ID3D11Buffer* m_pPixelBuffer;
};