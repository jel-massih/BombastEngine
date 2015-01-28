#include "D3DClass.h"
#include "../Bombast/BombastApp.h"

D3DClass11::D3DClass11()
{
	m_pSwapChain = 0;
	m_pDevice = 0;
	m_pDeviceContext = 0;
	m_pRenderTargetView = 0;
	m_pDepthStencilBuffer = 0;
	m_pDepthStencilState = 0;
	m_pDepthDisabledStencilState = 0;
	m_pDepthStencilView = 0;
	m_pRasterState = 0;
	m_pAlphaEnableBlendingState = 0;
	m_pAlphaDisabledBlendingState = 0;
}

D3DClass11::~D3DClass11()
{
	if (m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(false, NULL);
	}

	SAFE_RELEASE(m_pAlphaEnableBlendingState);
	SAFE_RELEASE(m_pAlphaDisabledBlendingState);
	SAFE_RELEASE(m_pRasterState);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthDisabledStencilState);
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pDepthStencilBuffer);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pSwapChain);
}

bool D3DClass11::VInitialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDesc;

	m_vsyncEnabled = true;
	m_bFullscreen = fullscreen;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))
	{
		return false;
	}

	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
	{
		return false;
	}

	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result))
	{
		return false;
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))
	{
		return false;
	}

	displayModeList = BE_NEW DXGI_MODE_DESC[numModes];
	if(!displayModeList)
	{
		return false;
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))
	{
		return false;
	}

	bool bValidResolution = false;
	
	for(i=0; i<numModes; i++)
	{
		if(displayModeList[i].Width == (unsigned int) screenWidth)
		{
			if(displayModeList[i].Height == (unsigned int) screenHeight)
			{
				bValidResolution = true;
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	if (!bValidResolution)
	{
		BE_ERROR("ERROR: Unsupported Resolution!");
		return false;
	}

	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
	{
		return false;
	}

	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
	{
		return false;
	}

	SAFE_DELETE_ARRAY(displayModeList);

	SAFE_RELEASE(adapterOutput);

	SAFE_RELEASE(adapter);

	SAFE_RELEASE(factory);

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if(m_vsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	
	swapChainDesc.OutputWindow = hwnd;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if(fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
		D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);
	if(FAILED(result))
	{
		return false;
	}

	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		return false;
	}

	result = m_pDevice->CreateRenderTargetView(backBufferPtr, NULL, &m_pRenderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	backBufferPtr->Release();
	backBufferPtr = 0;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0XFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	if(FAILED(result))
	{
		return false;
	}

	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	if(FAILED(result))
	{
		return false;
	}

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = false;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);
	if(FAILED(result))
	{
		return false;
	}

	m_pDeviceContext->RSSetState(m_pRasterState);

	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_pDeviceContext->RSSetViewports(1, &viewport);

	fieldOfView = (float)BE_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	DirectX::XMStoreFloat4x4(&m_projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth));

	DirectX::XMStoreFloat4x4(&m_worldMatrix, DirectX::XMMatrixIdentity());

	DirectX::XMStoreFloat4x4(&m_orthoMatrix, DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth));

	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0XFF;

	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_pDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &m_pDepthDisabledStencilState);
	if(FAILED(result))
	{
		return false;
	}

	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = m_pDevice->CreateBlendState(&blendStateDesc, &m_pAlphaEnableBlendingState);
	if(FAILED(result))
	{
		return false;
	}

	blendStateDesc.RenderTarget[0].BlendEnable = FALSE;

	result = m_pDevice->CreateBlendState(&blendStateDesc, &m_pAlphaDisabledBlendingState);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void D3DClass11::VSetBackgroundColor(float a, float r, float g, float b)
{
	m_backgroundColor[0] = r;
	m_backgroundColor[1] = g;
	m_backgroundColor[2] = b;
	m_backgroundColor[3] = a;
}

bool D3DClass11::VBeginScene()
{
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, m_backgroundColor);

	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	return true;
}

bool D3DClass11::VEndScene()
{
	if(m_vsyncEnabled)
	{
		m_pSwapChain->Present(1,0);
	}
	else
	{
		m_pSwapChain->Present(0, 0);
	}

	return true;
}

HRESULT D3DClass11::VOnRestore()
{
	return S_OK;
}

void D3DClass11::VSetWorldTransform(const Mat4x4* m)
{
	m_worldMatrix = *const_cast<Mat4x4*>(m);
}

void D3DClass11::VSetOrthoTransform(const Mat4x4* m)
{
	m_orthoMatrix = *const_cast<Mat4x4*>(m);
}

void D3DClass11::VSetProjectionTransform(const Mat4x4* m)
{
	m_projectionMatrix = *const_cast<Mat4x4*>(m);
}

void D3DClass11::VSetViewTransform(const Mat4x4* m)
{
	m_viewMatrix = *const_cast<Mat4x4*>(m);
}

void D3DClass11::VGetProjectionMatrix(Mat4x4& projMatrix)
{
	projMatrix = m_projectionMatrix;
	return;
}

void D3DClass11::VGetWorldMatrix(Mat4x4& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}

void D3DClass11::VGetOrthoMatrix(Mat4x4& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

void D3DClass11::VGetViewMatrix(Mat4x4& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

ID3D11Device* D3DClass11::GetDevice()
{
	return m_pDevice;
}

ID3D11DeviceContext* D3DClass11::GetDeviceContext()
{
	return m_pDeviceContext;
}

void D3DClass11::VGetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

void D3DClass11::VEnableZBuffer(bool bEnable)
{
	if(bEnable)
	{
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	}
	else
	{
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthDisabledStencilState, 1);
	}

	return;
}

void D3DClass11::VEnableAlphaBlending(bool bEnable)
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	if(bEnable)
	{
		m_pDeviceContext->OMSetBlendState(m_pAlphaEnableBlendingState, blendFactor, 0xffffffff);
	}
	else
	{
		m_pDeviceContext->OMSetBlendState(m_pAlphaDisabledBlendingState, blendFactor, 0xffffffff);
	}
}

void D3DClass11::VToggleFillMode()
{
	ID3D11RasterizerState * rState;
	D3D11_RASTERIZER_DESC rDesc;

	m_pDeviceContext->RSGetState(&rState);
	rState->GetDesc(&rDesc);

	rDesc.FillMode = (rDesc.FillMode == D3D11_FILL_SOLID) ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;

	m_pDevice->CreateRasterizerState(&rDesc, &rState);
	m_pDeviceContext->RSSetState(rState);
}

void D3DClass11::VToggleFullscreen()
{
	if (m_pSwapChain)
	{
		m_bFullscreen = !m_bFullscreen;
		m_pSwapChain->SetFullscreenState(m_bFullscreen, NULL);
	}
}

void D3DClass11::VPrepDeferredRendering() const
{
	g_pApp->GetGraphicsManager()->GetDeferredRenderingManager()->StartRender(m_pDevice, m_pDeviceContext, m_pRenderTargetView, m_pDepthStencilView);
}

void D3DClass11::VPrepForwardRendering() const
{
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, m_backgroundColor);
}