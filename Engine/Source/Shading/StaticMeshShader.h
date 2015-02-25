#pragma once

class Material;

class StaticMeshShader
{

public:
	StaticMeshShader();
	~StaticMeshShader();

	bool Initialize(ID3D11Device* device);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, const Material* material, const Scene* pScene);

private:
	bool InitializeShader(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, const Material* material, const Scene* pScene);
	void RenderShader(ID3D11DeviceContext* deviceContext, int);

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pCameraBuffer;
	ID3D11Buffer* m_pLightBuffer;
	ID3D11Buffer* m_pMaterialBuffer;
	ID3D11SamplerState* m_pSampleState;
};