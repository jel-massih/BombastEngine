#pragma once

#include "../BombastEngineStd.h"

class D3DClass
{
public:
	D3DClass();
	~D3DClass();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool bFullscreen, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(DirectX::XMMATRIX& projMatrix);
	void GetWorldMatrix(DirectX::XMMATRIX& worldMatrix);
	void GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix);

	void GetVideoCardInfo(char* cardName, int& memory);

	void EnableZBuffer(bool bEnable);

	void EnableAlphaBlending(bool bEnable);

	void ToggleFillMode();

private:
	bool m_vsyncEnabled;

	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;

	_declspec(align(16)) DirectX::XMMATRIX m_projectionMatrix;
	_declspec(align(16)) DirectX::XMMATRIX m_worldMatrix;
	_declspec(align(16)) DirectX::XMMATRIX m_orthoMatrix;

	ID3D11DepthStencilState* m_depthDisabledStencilState;
	
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisabledBlendingState;
};