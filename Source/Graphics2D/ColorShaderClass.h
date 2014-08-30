#ifndef COLOR_SHADER_CLASS_H
#define COLOR_SHADER_CLASS_H

#include "../BombastEngineStd.h"
#include <d3dcompiler.h>


class ColorShaderClass
{
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	bool Render(ID3D11DeviceContext* context, int indexCount, DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj);

private:
	bool InitializeShader(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext* context, DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj);
	void RenderShader(ID3D11DeviceContext* context, int indexCount);

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;
};

#endif