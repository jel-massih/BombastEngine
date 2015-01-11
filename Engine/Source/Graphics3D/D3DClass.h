#pragma once

#include "../BombastEngineStd.h"
#include "../Bombast/interfaces.h"

struct ConstantBuffer_Matrices
{
	Mat4x4 m_worldViewProj;
	Mat4x4 m_world;
};

struct ConstantBuffer_Material
{
	Vec4 m_diffuseObjectColor;
	Vec4 m_ambientObjectColor;
	bool m_bHasTexture;
	Vec3 m_unused;
};

#define MAXIMUM_LIGHTS_SUPPORTED (16)

struct ConstantBuffer_Lighting
{
	Vec4 m_lightDiffuse[MAXIMUM_LIGHTS_SUPPORTED];
	Vec4 m_lightDir[MAXIMUM_LIGHTS_SUPPORTED];
	Vec4 m_lightAmbient;
	unsigned int m_numLights;
	Vec3 m_unused;
};

class D3DClass11 : public IRenderer
{
public:
	D3DClass11();
	~D3DClass11();

	bool VInitialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool bFullscreen, float screenDepth, float screenNear);
	void VShutdown();
	
	void VSetBackgroundColor(float a, float r, float g, float b);

	bool VBeginScene();
	bool VEndScene();

	HRESULT VOnRestore();

	void VSetWorldTransform(const Mat4x4* m);
	void VSetOrthoTransform(const Mat4x4* m);
	void VSetProjectionTransform(const Mat4x4* m);
	void VSetViewTransform(const Mat4x4* m);

	void VGetProjectionMatrix(Mat4x4& projMatrix);
	void VGetWorldMatrix(Mat4x4& worldMatrix);
	void VGetOrthoMatrix(Mat4x4& orthoMatrix);
	void VGetViewMatrix(Mat4x4& viewMatrix);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void VGetVideoCardInfo(char* cardName, int& memory);

	void VEnableZBuffer(bool bEnable);
	void VEnableAlphaBlending(bool bEnable);
	void VToggleFillMode();

	void VToggleFullscreen();

	virtual void VCalcLighting(std::list<LightNode*>* lights, int maxLights) {}

private:
	bool m_vsyncEnabled;
	bool m_bFullscreen;

	int m_videoCardMemory;
	char m_videoCardDescription[128];
	float m_backgroundColor[4];

	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilState* m_pDepthDisabledStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RasterizerState* m_pRasterState;

	Mat4x4 m_projectionMatrix;
	Mat4x4 m_worldMatrix;
	Mat4x4 m_orthoMatrix;
	Mat4x4 m_viewMatrix;

	
	ID3D11BlendState* m_pAlphaEnableBlendingState;
	ID3D11BlendState* m_pAlphaDisabledBlendingState;
};