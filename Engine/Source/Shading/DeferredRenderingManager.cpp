#include "DeferredRenderingManager.h"

DeferredRenderingManager::DeferredRenderingManager()
	: m_pGbufferInputLayout(nullptr),
	m_pFillGBufferVertexShader(nullptr),
	m_pFillGBufferPixelShader(nullptr),
	m_pLightingVertexShader(nullptr),
	m_pLightingPixelShader(nullptr),
	m_pLightingInputLayout(nullptr),
	m_pTextureSampler(nullptr),
	m_pMatrixBuffer(nullptr)
{
}

DeferredRenderingManager::~DeferredRenderingManager()
{
	SAFE_RELEASE(m_pGbufferInputLayout);
	SAFE_RELEASE(m_pFillGBufferVertexShader);
	SAFE_RELEASE(m_pFillGBufferPixelShader);

	SAFE_RELEASE(m_pLightingInputLayout);
	SAFE_RELEASE(m_pLightingVertexShader);
	SAFE_RELEASE(m_pLightingPixelShader);
	
	SAFE_RELEASE(m_pTextureSampler);
	SAFE_RELEASE(m_pLightPointSampler);

	SAFE_RELEASE(m_pMatrixBuffer);

	m_texGBuffer.Release();
	m_texGBuffer2.Release();
}

bool DeferredRenderingManager::Initialize(ID3D11Device* device, int texWidth, int texHeight)
{
	HRESULT result;

	if (!InitializeGBufferShader(device)) {
		return false;
	}

	if (!InitializeLightShader(device)) {
		return false;
	}

	m_texGBuffer.Initialize(device, DXGI_FORMAT_R32G32B32A32_FLOAT, texWidth, texHeight, true, true);
	m_texGBuffer2.Initialize(device, DXGI_FORMAT_R32G32B32A32_FLOAT, texWidth, texHeight, true, true);

	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = sampDesc.AddressV = sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.MaxAnisotropy = 0;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = FLT_MAX;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

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
			BE_ERROR("Error: Failed to create ");
			return false;
		}
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
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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

bool DeferredRenderingManager::InitializeLightShader(ID3D11Device* device)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;

	Resource vertexShaderResource("Shaders\\DeferredLightingVertexShader.cso");
	ResourceHandle* pVertexResHandle = g_pApp->m_pResourceCache->GetHandle(&vertexShaderResource);

	result = device->CreateVertexShader(pVertexResHandle->Buffer(), pVertexResHandle->Size(), nullptr, &m_pLightingVertexShader);
	if (FAILED(result))
	{
		BE_ERROR("Failed to create vertex shader");

		return false;
	}

	Resource pixelShaderResource("Shaders\\DeferredLightingPixelShader.cso");
	ResourceHandle* pPixelResHandle = g_pApp->m_pResourceCache->GetHandle(&pixelShaderResource);

	result = device->CreatePixelShader(pPixelResHandle->Buffer(), pPixelResHandle->Size(), nullptr, &m_pLightingPixelShader);
	if (FAILED(result))
	{
		BE_ERROR("Failed to create Pixel shader");

		return false;
	}

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, pVertexResHandle->Buffer(), pVertexResHandle->Size(), &m_pLightingInputLayout);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void DeferredRenderingManager::StartRender(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV) const
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;

	//Clear the Back Buffer
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	context->ClearRenderTargetView(pRTV, clearColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	{
		//Fill G Buffer
		ID3D11RenderTargetView* pGBufRTV[] = { m_texGBuffer.GetRenderTargetView(), m_texGBuffer2.GetRenderTargetView() };
		context->ClearRenderTargetView(pGBufRTV[0], clearColor);
		context->ClearRenderTargetView(pGBufRTV[1], clearColor);
	
		context->OMSetRenderTargets(2, pGBufRTV, pDSV);
		context->OMSetDepthStencilState(NULL, 0);

		context->IASetInputLayout(m_pGbufferInputLayout);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->VSSetShader(m_pFillGBufferVertexShader, NULL, 0);
		context->PSSetShader(m_pFillGBufferPixelShader, NULL, 0);

		context->PSSetSamplers(0, 1, &m_pTextureSampler);
	}
}

void DeferredRenderingManager::DrawRenderable(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix) const
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(m_pMatrixBuffer, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);
	
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

void DeferredRenderingManager::FinishRender(ID3D11DeviceContext* context, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV) const
{
	ID3D11RenderTargetView* pClearRTV[] = { NULL, NULL, NULL };
	context->OMSetRenderTargets(3, pClearRTV, NULL);

	//Calculate Lighting
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	context->OMSetRenderTargets(1, &pRTV, pDSV);

	context->VSSetShader(m_pLightingVertexShader, NULL, 0);
	context->PSSetShader(m_pFillGBufferPixelShader, NULL, 0);

	//Setup Lighting Pass Matrix's
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		//context->Map()
	}
}