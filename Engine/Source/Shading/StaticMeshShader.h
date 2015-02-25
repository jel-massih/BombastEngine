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
	bool InitializeShaders(ID3D11Device* device);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, int indexCount, const Material* material, const Scene* pScene);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount, ID3D11PixelShader* program);

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pCameraBuffer;
	ID3D11Buffer* m_pLightBuffer;
	ID3D11Buffer* m_pMaterialBuffer;
	ID3D11SamplerState* m_pSampleState;

	class ProgramImpl;
	ProgramImpl* m_pProgramImpl;
};