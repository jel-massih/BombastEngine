#include "DeferredRenderingManager.h"
#include "../Graphics2D/PostProcessRenderWindow.h"
#include "DeferredLightShader.h"

DeferredRenderingManager::DeferredRenderingManager()
	: m_pGbufferInputLayout(nullptr),
	m_pFillGBufferVertexShader(nullptr),
	m_pFillGBufferPixelShader(nullptr),
	m_pTextureSampler(nullptr),
	m_pMatrixBuffer(nullptr),
	m_pDepthStencilBuffer(nullptr),
	m_pDepthStencilView(nullptr),
	m_pDeferredLightShader(nullptr),
	m_pPostProcessRenderWindow(nullptr)
{
}

DeferredRenderingManager::~DeferredRenderingManager()
{
	SAFE_RELEASE(m_pGbufferInputLayout);
	SAFE_RELEASE(m_pFillGBufferVertexShader);
	SAFE_RELEASE(m_pFillGBufferPixelShader);

	SAFE_RELEASE(m_pTextureSampler);

	SAFE_RELEASE(m_pMatrixBuffer);
	SAFE_RELEASE(m_pDepthStencilBuffer);
	SAFE_RELEASE(m_pDepthStencilView);

	SAFE_DELETE(m_pDeferredLightShader);
	SAFE_DELETE(m_pPostProcessRenderWindow);

	m_texGBuffer.Release();
	m_texGBuffer2.Release();
}

bool DeferredRenderingManager::Initialize(ID3D11Device* device, int texWidth, int texHeight)
{
	HRESULT result;

	if (!InitializeGBufferShader(device)) {
		return false;
	}

	m_texGBuffer.Initialize(device, DXGI_FORMAT_R32G32B32A32_FLOAT, texWidth, texHeight, true, true);
	m_texGBuffer2.Initialize(device, DXGI_FORMAT_R32G32B32A32_FLOAT, texWidth, texHeight, true, true);

	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = sampDesc.AddressV = sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.MipLODBias = 0.0f;
		sampDesc.MaxAnisotropy = 1;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.BorderColor[0] = 0;
		sampDesc.BorderColor[1] = 0;
		sampDesc.BorderColor[2] = 0;
		sampDesc.BorderColor[3] = 0;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = FLT_MAX;

		result = device->CreateSamplerState(&sampDesc, &m_pTextureSampler);
		if (FAILED(result)) {
			BE_ERROR("Error: Failed to create Point samplerstate");
			return false;
		}
	}

	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.ByteWidth = sizeof(MatrixBufferType);

		result = device->CreateBuffer(&bd, NULL, &m_pMatrixBuffer);
		if (FAILED(result)) {
			BE_ERROR("Error: Failed to create Matrix Buffer");
			return false;
		}
	}

	{
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

		depthBufferDesc.Width = texWidth;
		depthBufferDesc.Height = texHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
		if (FAILED(result)) {
			BE_ERROR("Error: Failed to create Depth Buffer");
			return false;
		}
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		result = device->CreateDepthStencilView(m_pDepthStencilBuffer, &dsvDesc, &m_pDepthStencilView);
		if (FAILED(result)) {
			BE_ERROR("Error: Failed to create Depth Stencil View");
			return false;
		}
	}

	m_pDeferredLightShader = BE_NEW DeferredLightShader;
	if (!m_pDeferredLightShader)
	{
		BE_ERROR("Could not Allocate the DeferredLightShader Object!");
		return false;
	}

	result = m_pDeferredLightShader->Initialize(device);
	if (!result)
	{
		BE_ERROR("Could not initialize the DeferredLightShader Object!");
		return false;
	}

	m_pPostProcessRenderWindow = BE_NEW PostProcessRenderWindow;
	if (!m_pPostProcessRenderWindow)
	{
		BE_ERROR("Could not Allocate the PostProcessRenderWindow Object!");
		return false;
	}

	Point screenSize = g_pApp->GetScreenSize();
	result = m_pPostProcessRenderWindow->Initialize(device, screenSize.GetX(), screenSize.GetY());
	if (!result)
	{
		BE_ERROR("Could not initialize the PostProcessRenderWindow Object!");
		return false;
	}

	return true;
}

bool DeferredRenderingManager::InitializeGBufferShader(ID3D11Device* device)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;

	Resource vertexShaderResource("Shaders\\FillGBufferVertexShader.cso");
	ResourceHandle* pVertexResHandle = g_pApp->m_pResourceCache->GetHandle(&vertexShaderResource);

	result = device->CreateVertexShader(pVertexResHandle->Buffer(), pVertexResHandle->Size(), nullptr, &m_pFillGBufferVertexShader);
	if (FAILED(result))
	{
		BE_ERROR("Failed to create vertex shader");

		return false;
	}

	Resource pixelShaderResource("Shaders\\FillGBufferPixelShader.cso");
	ResourceHandle* pPixelResHandle = g_pApp->m_pResourceCache->GetHandle(&pixelShaderResource);

	result = device->CreatePixelShader(pPixelResHandle->Buffer(), pPixelResHandle->Size(), nullptr, &m_pFillGBufferPixelShader);
	if (FAILED(result))
	{
		BE_ERROR("Failed to create Pixel shader");

		return false;
	}


	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	//Get count of elements in layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//Create vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, pVertexResHandle->Buffer(), pVertexResHandle->Size(), &m_pGbufferInputLayout);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void DeferredRenderingManager::StartRender(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV) const
{
	HRESULT result;

	//Clear the Back Buffer
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	{
		//Fill G Buffer
		ID3D11RenderTargetView* pGBufRTV[] = { m_texGBuffer.GetRenderTargetView(), m_texGBuffer2.GetRenderTargetView() };
		context->OMSetRenderTargets(2, pGBufRTV, pDSV);


		context->ClearRenderTargetView(pGBufRTV[0], clearColor);
		context->ClearRenderTargetView(pGBufRTV[1], clearColor);
		context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);


		context->IASetInputLayout(m_pGbufferInputLayout);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->VSSetShader(m_pFillGBufferVertexShader, NULL, 0);
		context->PSSetShader(m_pFillGBufferPixelShader, NULL, 0);

		context->PSSetSamplers(0, 1, &m_pTextureSampler);
	}
}

void DeferredRenderingManager::DrawRenderable(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture) const
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(m_pMatrixBuffer, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);
	deviceContext->PSSetShaderResources(0, 1, &texture);
	
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

void DeferredRenderingManager::DrawLightPass(ID3D11DeviceContext* context, const Scene* pScene) const
{
	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();

	Mat4x4 worldMatrix, projectionMatrix, orthoMatrix;
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetOrthoMatrix(orthoMatrix);

	m_pPostProcessRenderWindow->Render(context);
	m_pDeferredLightShader->Render(context, m_pPostProcessRenderWindow->GetIndexCount(), XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&Mat4x4::g_Identity), XMLoadFloat4x4(&orthoMatrix), m_texGBuffer.GetShaderResourceView(), m_texGBuffer2.GetShaderResourceView(), pScene);
}