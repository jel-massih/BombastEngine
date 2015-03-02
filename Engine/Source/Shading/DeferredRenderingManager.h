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

	void StartRender(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV, const float* clearBufferColor) const;
	
	void DrawLightPass(ID3D11DeviceContext* context, const Scene* pScene) const;

	const SimpleTexture& GetColorRenderTexture() const { return m_texGBuffer; }
	const SimpleTexture& GetNormalRenderTexture() const { return m_texGBuffer2; }

private:

	SimpleTexture m_texGBuffer;
	SimpleTexture m_texGBuffer2;

	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;

	DeferredLightShader* m_pDeferredLightShader;
	PostProcessRenderWindow* m_pPostProcessRenderWindow;
};