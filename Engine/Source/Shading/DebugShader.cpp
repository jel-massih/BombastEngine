#include "DebugShader.h"
#include "../Resources/ShaderResource.h"

DebugShader::DebugShader()
{
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pLayout = 0;
	m_pConstantBuffer = 0;
	m_pSampleState = 0;
	m_pPixelBuffer = 0;
}

DebugShader::~DebugShader()
{
	SAFE_RELEASE(m_pPixelBuffer);
	SAFE_RELEASE(m_pSampleState);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pLayout);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);
}

bool DebugShader::Initialize(ID3D11Device* device)
{
	if (!InitializeShader(device, "EngineResources.Shaders.DebugVertexShader.cso", "EngineResources.Shaders.DebugPixelShader.cso"))
	{
		return false;
	}

	return true;
}

bool DebugShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix,
	XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, XMFLOAT4 pixelColor)
{
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, pixelColor))
	{
		return false;
	}

	RenderShaders(deviceContext, indexCount);

	return true;
}

bool DebugShader::InitializeShader(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC constantBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC pixelBufferDesc;

	Resource vertexShaderResource(vertexShaderPath.c_str());
	ResourceHandle* pVertexResHandle = g_pApp->m_pResourceCache->GetHandle(&vertexShaderResource);

	result = device->CreateVertexShader(pVertexResHandle->Buffer(), pVertexResHandle->Size(), nullptr, &m_pVertexShader);
	if (FAILED(result))
	{
		BE_ERROR("Failed to create Debug Vertex Shader");

		return false;
	}

	Resource pixelShaderResource(pixelShaderPath.c_str());
	ResourceHandle* pPixelResHandle = g_pApp->m_pResourceCache->GetHandle(&pixelShaderResource);

	result = device->CreatePixelShader(pPixelResHandle->Buffer(), pPixelResHandle->Size(), nullptr, &m_pPixelShader);
	if (FAILED(result))
	{
		BE_ERROR("Failed to create Debug Pixel Shader");

		return false;
	}

	polygonLayout[0].SemanticName = "Position";
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

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, pVertexResHandle->Buffer(), pVertexResHandle->Size(), &m_pLayout);
	if (FAILED(result))
	{
		return false;
	}

	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferType);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&constantBufferDesc, NULL, &m_pConstantBuffer);
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &m_pSampleState);
	if (FAILED(result))
	{
		return false;
	}

	pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
	pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pixelBufferDesc.MiscFlags = 0;
	pixelBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&pixelBufferDesc, NULL, &m_pPixelBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool DebugShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX& world, XMMATRIX& view, XMMATRIX& projection, XMFLOAT4 pixelColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ConstantBufferType* dataPtr;
	unsigned int bufferNumber;
	PixelBufferType* dataPtr2;
	XMFLOAT4X4A tempAlignedMatrix;

	result = deviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (ConstantBufferType*)mappedResource.pData;

	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	deviceContext->Unmap(m_pConstantBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pConstantBuffer);

	result = deviceContext->Map(m_pPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (PixelBufferType*)mappedResource.pData;
	dataPtr2->color = pixelColor;

	deviceContext->Unmap(m_pPixelBuffer, 0);

	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pPixelBuffer);

	return true;
}

void DebugShader::RenderShaders(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(m_pLayout);

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &m_pSampleState);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}