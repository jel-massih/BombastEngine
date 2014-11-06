#pragma once
//P. 508,515

//Forward Declares
class Scene;
class SceneNode;

class BombastEngine_Hlsl_VertexShader
{
public:
	BombastEngine_Hlsl_VertexShader();
	~BombastEngine_Hlsl_VertexShader();

	HRESULT OnRestore(Scene* pScene);
	HRESULT SetupRender(Scene* pScene, SceneNode* pNode);
	void EnableLights(bool enableLights) { m_bEnableLights = enableLights; }

protected:
	ID3D11InputLayout* m_pVertexLayout11;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11Buffer* m_pcbVSMatrices;
	ID3D11Buffer* m_pcbVSLighting;
	ID3D11Buffer* m_pcbVSMaterial;
	bool m_bEnableLights;
};

class BombastEngine_Hlsl_PixelShader
{
public:
	BombastEngine_Hlsl_PixelShader();
	~BombastEngine_Hlsl_PixelShader();

	HRESULT OnRestore(Scene* pScene);
	HRESULT SetupRender(Scene* pScene, SceneNode* pNode);
	HRESULT SetTexture(const std::string& textureName);
	HRESULT SetTexture(ID3D11ShaderResourceView* const *pDiffuseRV, ID3D11SamplerState* const* ppSamplers);

protected:
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pcbPSMaterial;
	std::string m_textureResource;
};

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectPass;