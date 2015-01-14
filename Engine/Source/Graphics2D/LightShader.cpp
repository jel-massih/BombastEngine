#include "LightShader.h"
#include "../Graphics3D/Material.h"

LightShader::LightShader()
{
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pLayout = nullptr;
	m_pMatrixBuffer = nullptr;
	m_pCameraBuffer = nullptr;
	m_pLightBuffer = nullptr;
	m_pSampleState = nullptr;
}

bool LightShader::Initialize(ID3D11Device* device)
{
	bool result;

	result = InitializeShader(device, "Shaders\\LitTexturedVertexShader.cso", "Shaders\\LitTexturedPixelShader.cso");
	if (!result)
	{
		return false;
	}

	return true;
}

void LightShader::Shutdown()
{
	ShutdownShader();

	return;
}

bool LightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX &world, DirectX::XMMATRIX &view, DirectX::XMMATRIX &projection, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT3 lightDirection, DirectX::XMFLOAT4 diffuseColor, Vec3 cameraPosition, const Material* material)
{
	bool result;

	result = SetShaderParameters(deviceContext, world, view, projection, texture, lightDirection, diffuseColor, cameraPosition, material);
	if (!result)
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightShader::InitializeShader(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC materialBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	Resource vertexShaderResource(vertexShaderPath.c_str());
	ResourceHandle* pVertexResHandle = g_pApp->m_pResourceCache->GetHandle(&vertexShaderResource);

	result = device->CreateVertexShader(pVertexResHandle->Buffer(), pVertexResHandle->Size(), nullptr, &m_pVertexShader);
	if (FAILED(result))
	{
		BE_ERROR("Failed to create vertex shader");

		return false;
	}

	Resource pixelShaderResource(pixelShaderPath.c_str());
	ResourceHandle* pPixelResHandle = g_pApp->m_pResourceCache->GetHandle(&pixelShaderResource);

	result = device->CreatePixelShader(pPixelResHandle->Buffer(), pPixelResHandle->Size(), nullptr, &m_pPixelShader);
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

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	//Get count of elements in layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//Create vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, pVertexResHandle->Buffer(), pVertexResHandle->Size(), &m_pLayout);
	if (FAILED(result))
	{
		return false;
	}

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

	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_pCameraBuffer);
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

	materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	materialBufferDesc.ByteWidth = sizeof(MaterialBufferType);
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufferDesc.MiscFlags = 0;
	materialBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&materialBufferDesc, NULL, &m_pMaterialBuffer);
	if (FAILED(result))
	{
		return false;
	}

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_pLightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void LightShader::ShutdownShader()
{
	SAFE_RELEASE(m_pSampleState);
	SAFE_RELEASE(m_pLayout);
	SAFE_RELEASE(m_pLightBuffer);
	SAFE_RELEASE(m_pMaterialBuffer);
	SAFE_RELEASE(m_pCameraBuffer);
	SAFE_RELEASE(m_pMatrixBuffer);
	SAFE_RELEASE(m_pLayout);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);

	return;
}

bool LightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX &world, DirectX::XMMATRIX &view, DirectX::XMMATRIX &projection, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT3 lightDirection, DirectX::XMFLOAT4 diffuseColor, Vec3 cameraPosition, const Material* material)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	CameraBufferType* dataPtr2;
	MaterialBufferType* dataPtr3;
	LightBufferType* dataPtr4;
	unsigned int bufferNumber;

	world = DirectX::XMMatrixTranspose(world);
	view = DirectX::XMMatrixTranspose(view);
	projection = DirectX::XMMatrixTranspose(projection);

	result = deviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	deviceContext->Unmap(m_pMatrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	result = deviceContext->Map(m_pCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (CameraBufferType*)mappedResource.pData;

	dataPtr2->cameraPosition = cameraPosition;
	dataPtr2->padding = 0.0f;

	deviceContext->Unmap(m_pCameraBuffer, 0);

	bufferNumber = 1;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pCameraBuffer);

	result = deviceContext->Map(m_pMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr3 = (MaterialBufferType*)mappedResource.pData;

	dataPtr3->ambient = material->GetAmbient();
	dataPtr3->diffuse = material->GetDiffuse();
	dataPtr3->emissive = material->GetEmissive();
	dataPtr3->useTexture = true;
	dataPtr3->padding = XMFLOAT2(0, 0);
	Vec4 specular;
	material->GetSpecular(specular, dataPtr3->specularPower);
	dataPtr3->specular = specular;

	deviceContext->Unmap(m_pMaterialBuffer, 0);

	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pMaterialBuffer);

	result = deviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr4 = (LightBufferType*)mappedResource.pData;

	dataPtr4->eyePosition = Vec4(cameraPosition, 0.0f);

	deviceContext->Unmap(m_pLightBuffer, 0);
	bufferNumber = 1;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pLightBuffer);


	//set shader texture resource in pixel shader
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void LightShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
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