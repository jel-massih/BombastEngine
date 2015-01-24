#include "DeferredRenderingManager.h"

DeferredRenderingManager::DeferredRenderingManager()
	: m_pGbufferInputLayout(nullptr),
	m_pFillGBufferVertexShader(nullptr),
	m_pFillGBufferPixelShader(nullptr),
	m_pTextureSampler(nullptr)
{
}

DeferredRenderingManager::~DeferredRenderingManager()
{
	SAFE_RELEASE(m_pGbufferInputLayout);
	SAFE_RELEASE(m_pFillGBufferVertexShader);
	SAFE_RELEASE(m_pFillGBufferPixelShader);
	
	SAFE_RELEASE(m_pTextureSampler);
	SAFE_RELEASE(m_pPointSampler);
	SAFE_RELEASE(m_pWriteStencilState);
	SAFE_RELEASE(m_pTestStencilState);

	m_texGBuffer.Release();
	m_texGBuffer2.Release();
	m_texGBufResolved2.Release();
	m_texCoverageMask.Release();
}

bool DeferredRenderingManager::Initialize(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath, int texWidth, int texHeight)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;

	Resource vertexShaderResource(vertexShaderPath.c_str());
	ResourceHandle* pVertexResHandle = g_pApp->m_pResourceCache->GetHandle(&vertexShaderResource);

	result = device->CreateVertexShader(pVertexResHandle->Buffer(), pVertexResHandle->Size(), nullptr, &m_pFillGBufferVertexShader);
	if (FAILED(result))
	{
		BE_ERROR("Failed to create vertex shader");

		return false;
	}

	Resource pixelShaderResource(pixelShaderPath.c_str());
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

	{
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		dsDesc.DepthEnable = false;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace = dsDesc.FrontFace;

		result = device->CreateDepthStencilState(&dsDesc, &m_pWriteStencilState);
		if (FAILED(result)) {
			BE_ERROR("Error: Failed to Create WriteStencil DepthStencilState");
			return false;
		}
	}

	{
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		dsDesc.DepthEnable = false;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = 0;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		dsDesc.BackFace = dsDesc.FrontFace;

		result = device->CreateDepthStencilState(&dsDesc, &m_pTestStencilState);
		if (FAILED(result)) {
			BE_ERROR("Error: Failed to Create TestStencil DepthStencilState");
			return false;
		}
	}

	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
		sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampDesc.AddressU = sampDesc.AddressV = sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.MaxAnisotropy = 16;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = FLT_MAX;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		result = device->CreateSamplerState(&sampDesc, &m_pTextureSampler);
		if (FAILED(result)) {
			BE_ERROR("Error: Failed to Create Texture SamplerState");
			return false;
		}
	}

	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = sampDesc.AddressV = sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.MaxAnisotropy = 0;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = FLT_MAX;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		result = device->CreateSamplerState(&sampDesc, &m_pPointSampler);
		if (FAILED(result)) {
			BE_ERROR("Error: Failed to create Point samplerstate");
			return false;
		}
	}

	return true;
}

void DeferredRenderingManager::Render(ID3D11Device* device, ID3D11DeviceContext* context, DirectX::XMMATRIX &world, DirectX::XMMATRIX &view, DirectX::XMMATRIX &projection, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;

	world = DirectX::XMMatrixTranspose(world);
	view = DirectX::XMMatrixTranspose(view);
	projection = DirectX::XMMatrixTranspose(projection);

	//Clear the Back Buffer
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	context->ClearRenderTargetView(pRTV, clearColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	{
		//Fill G Buffer
		
		ID3D11RenderTargetView* pGBufRTV[] = { m_texGBuffer.GetRenderTargetView(), m_texGBuffer2.GetRenderTargetView(), m_texCoverageMask.GetRenderTargetView() };
		context->ClearRenderTargetView(pGBufRTV[0], clearColor);
		context->ClearRenderTargetView(pGBufRTV[1], clearColor);
		context->ClearRenderTargetView(pGBufRTV[2], clearColor);
	
		context->OMSetRenderTargets(3, pGBufRTV, pDSV);
		context->OMSetDepthStencilState(NULL, 0);

		context->IASetInputLayout(m_pGbufferInputLayout);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->VSSetShader(m_pFillGBufferVertexShader, NULL, 0);
		context->PSSetShader(m_pFillGBufferPixelShader, NULL, 0);

		context->PSSetSamplers(0, 1, &m_pTextureSampler);

		//@TODO: Render Mesh [Draw Indexed]

		ID3D11RenderTargetView* pClearRTV[] = { NULL, NULL, NULL };
		context->OMSetRenderTargets(3, pClearRTV, NULL);
	}


}