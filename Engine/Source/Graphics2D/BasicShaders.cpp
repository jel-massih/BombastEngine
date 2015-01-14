#include "BasicShaders.h"

ColorShader::ColorShader()
{
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pLayout = 0;
	m_pConstantBuffer = 0;
	m_pPixelBuffer = 0;
}

bool ColorShader::Initialize(ID3D11Device* device)
{
	if (!InitializeShader(device, "Shaders\\ColorVertexShader.cbo", "Shaders\\ColorPixelShader.cbo"))
	{
		return false;
	}

	return true;
}

void ColorShader::Shutdown()
{
	ShutdownShader();
}

bool ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix,
	XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, XMFLOAT4 pixelColor)
{
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, pixelColor))
	{
		return false;
	}

	RenderShaders(deviceContext, indexCount);

	return true;
}

bool ColorShader::InitializeShader(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	unsigned int numElements;
	D3D11_BUFFER_DESC constantBufferDesc;
	D3D11_BUFFER_DESC pixelBufferDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	Resource vertexShaderResource(vertexShaderPath.c_str());
	ResourceHandle* pVertexResHandle = g_pApp->m_pResourceCache->GetHandle(&vertexShaderResource);
	ID3D11VertexShader* vertexShader;

	result = device->CreateVertexShader(reinterpret_cast<BYTE*>(pVertexResHandle->Buffer()), pVertexResHandle->Size(), nullptr, &vertexShader);
	if (FAILED(result))
	{
		BE_ERROR("Failed to create vertex shader");

		return false;
	}

	Resource pixelShaderResource(pixelShaderPath.c_str());
	ResourceHandle* pPixelResHandle = g_pApp->m_pResourceCache->GetHandle(&pixelShaderResource);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			BE_ERROR((char*)errorMessage->GetBufferPointer());
		}
		else
		{
			BE_ERROR("Missing Vertex Shader File");
		}

		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	polygonLayout[0].SemanticName = "Position";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_pLayout);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

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

void ColorShader::ShutdownShader()
{
	SAFE_RELEASE(m_pPixelBuffer);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pLayout);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX& world, XMMATRIX& view, XMMATRIX& projection, XMFLOAT4 pixelColor)
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

void ColorShader::RenderShaders(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(m_pLayout);

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}

TextureShader::TextureShader()
{
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pLayout = 0;
	m_pMatrixBuffer = 0;
	m_pSampleState = 0;
}

bool TextureShader::Initialize(ID3D11Device* device)
{
	bool result;

	result = InitializeShader(device, "Shaders\\TextureVertexShader.hlsl", "Shaders\\TexturePixelShader.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void TextureShader::Shutdown()
{
	ShutdownShader();

	return;
}

bool TextureShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX &world, DirectX::XMMATRIX &view, DirectX::XMMATRIX &projection, ID3D11ShaderResourceView* texture)
{
	bool result;

	result = SetShaderParameters(deviceContext, world, view, projection, texture);
	if (!result)
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool TextureShader::InitializeShader(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//Compile Vertex Shader
	Resource vertexShaderResource(vertexShaderPath.c_str());
	ResourceHandle* pVertexResHandle = g_pApp->m_pResourceCache->GetHandle(&vertexShaderResource);
	
	result = S_FALSE;

	if (FAILED(result))
	{
		if (errorMessage)
		{
			BE_ERROR((char*)errorMessage->GetBufferPointer());
		}
		else
		{
			BE_ERROR("Missing Vertex Shader File");
		}

		return false;
	}

	//Compile Pixel Shader
	Resource pixelShaderResource(pixelShaderPath.c_str());
	ResourceHandle* pPixelResHandle = g_pApp->m_pResourceCache->GetHandle(&pixelShaderResource);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			BE_ERROR((char*)errorMessage->GetBufferPointer());
		}
		else
		{
			BE_ERROR("Missing Pixel Shader File");
		}

		return false;
	}

	//Create Vertex Shader From Buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//Create Pixel Shader From Buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	//Create Vertex Input layout Description
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

	//Get count of elements in layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//Create vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pLayout);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//Setup the desc of dynamic matrix constant buffer thats in vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//Create constant buffer pointer so class can access vertex shader constant buffer
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Create Texture Sampler State Desc
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

	//Create Texture Sampler State
	result = device->CreateSamplerState(&samplerDesc, &m_pSampleState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureShader::ShutdownShader()
{
	SAFE_RELEASE(m_pSampleState);
	SAFE_RELEASE(m_pMatrixBuffer);
	SAFE_RELEASE(m_pLayout);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);

	return;
}

bool TextureShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX &world, DirectX::XMMATRIX &view, DirectX::XMMATRIX &projection, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	//Transpose Matrices to prep for Shader
	world = DirectX::XMMatrixTranspose(world);
	view = DirectX::XMMatrixTranspose(view);
	projection = DirectX::XMMatrixTranspose(projection);

	//Lock constant buffer so it can be written to
	result = deviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get pointer to data in constant buffer
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//Copy matrices into constant buffer
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	//unlock constant buffer
	deviceContext->Unmap(m_pMatrixBuffer, 0);

	//set position of constant buffer in vertex shader
	bufferNumber = 0;

	//set constant buffer in vertex shader with updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	//set shader texture resource in pixel shader
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void TextureShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//Set the vertex input layout.
	deviceContext->IASetInputLayout(m_pLayout);

	//Set the vertex and pixel shaders that will be used to render triangle
	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	//Set the sampler state in pixel shader
	deviceContext->PSSetSamplers(0, 1, &m_pSampleState);

	//Render the triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}