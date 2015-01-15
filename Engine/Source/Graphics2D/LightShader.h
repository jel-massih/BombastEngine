#pragma once

class Material;

class LightShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct MaterialBufferType
	{
		XMFLOAT4 emissive;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		float specularPower;
		bool useTexture;
		XMFLOAT2 padding;
	};

	struct LightProperties
	{
		XMFLOAT4 lightColor;
		XMFLOAT3 lightDirection;
		bool enabled;
	};

	struct LightBufferType
	{
		XMFLOAT4 eyePosition;
		XMFLOAT4 globalAmbient;
		LightProperties lights[MAX_LIGHTS_SUPPORTED];
	};

public:
	LightShader();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX &world, DirectX::XMMATRIX &view, DirectX::XMMATRIX &projection, const Material* material, const Scene* pScene);

private:
	bool InitializeShader(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX &world, DirectX::XMMATRIX &view, DirectX::XMMATRIX &projection, const Material* material, const Scene* pScene);
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