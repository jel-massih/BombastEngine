#ifndef TEXTURE_SHADER_CLASS_H
#define TEXTURE_SHADER_CLASS_H

#include "../BombastEngineStd.h"
#include "../Bombast/BombastApp.h"

#include <d3dcompiler.h>

class TextureShaderClass
{
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass& other);
	~TextureShaderClass();

	bool Initialize(ID3D11Device* device, HWND hWnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX &world, DirectX::XMMATRIX &view, DirectX::XMMATRIX &projection, ID3D11ShaderResourceView* texture);

private:
	bool InitializeShader(ID3D11Device* device, HWND hWnd, WCHAR* vertexShaderPath, WCHAR* pixelShaderPath);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX &world, DirectX::XMMATRIX &view, DirectX::XMMATRIX &projection, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* deviceContext, int);

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11SamplerState* m_pSampleState;
};

#endif