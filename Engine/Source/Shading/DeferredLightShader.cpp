#include "DeferredLightShader.h"
#include "../Graphics3D/Material.h"
#include "../Graphics3D/Scene.h"
#include "ShadingCommon.h"


DeferredLightShader::DeferredLightShader()
{
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pLayout = nullptr;
	m_pMatrixBuffer = nullptr;
	m_pLightBuffer = nullptr;
	m_pPointSampleState = nullptr;
	m_pDepthSampleState = nullptr;
	m_pCameraBuffer = nullptr;
}

DeferredLightShader::~DeferredLightShader()
{
	SAFE_RELEASE(m_pPointSampleState);
	SAFE_RELEASE(m_pLayout);
	SAFE_RELEASE(m_pLightBuffer);
	SAFE_RELEASE(m_pMatrixBuffer);
	SAFE_RELEASE(m_pLayout);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pDepthSampleState);
	SAFE_RELEASE(m_pCameraBuffer);
}

bool DeferredLightShader::Initialize(ID3D11Device* device)
{
	bool result;

	result = InitializeShader(device, "EngineResources.Shaders.DeferredLightingVertexShader.cso", "EngineResources.Shaders.DeferredLightingPixelShader.cso");
	if (!result)
	{
		return false;
	}

	return true;
}

bool DeferredLightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* depthTexture, const Scene* pScene)
{
	bool result;

	result = SetShaderParameters(deviceContext, colorTexture, normalTexture, depthTexture, pScene);
	if (!result)
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool DeferredLightShader::InitializeShader(ID3D11Device* device, std::string vertexShaderPath, std::string pixelShaderPath)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;

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

	{
		D3D11_SAMPLER_DESC samplerDesc;

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		result = device->CreateSamplerState(&samplerDesc, &m_pPointSampleState);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		D3D11_SAMPLER_DESC samplerDesc;

		samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[1] = 1.0f;
		samplerDesc.BorderColor[2] = 1.0f;
		samplerDesc.BorderColor[3] = 1.0f;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		result = device->CreateSamplerState(&samplerDesc, &m_pDepthSampleState);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
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
	}

	{
		cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cameraBufferDesc.ByteWidth = sizeof(beShading::CameraBufferType);
		cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cameraBufferDesc.MiscFlags = 0;
		cameraBufferDesc.StructureByteStride = 0;

		result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_pCameraBuffer);
		if (FAILED(result))
		{
			return false;
		}
	}

	return true;
}

bool DeferredLightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* depthTexture, const Scene* pScene)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	beShading::CameraBufferType* dataPtr3;
	unsigned int bufferNumber;


	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();

	Mat4x4 worldMatrix, orthoMatrix, projectionMatrix, viewMatrix;
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetViewMatrix(viewMatrix);
	pRenderer->VGetProjectionMatrix(projectionMatrix);
	pRenderer->VGetOrthoMatrix(orthoMatrix);

	XMMATRIX world = DirectX::XMMatrixTranspose(worldMatrix.Mat());
	XMMATRIX orthoProjection = DirectX::XMMatrixTranspose(orthoMatrix.Mat());

	result = deviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = world;
	dataPtr->orthoProjection = orthoProjection;
	dataPtr->InverseViewProjectionMatrix = XMMatrixInverse(nullptr, (viewMatrix * projectionMatrix).Mat());

	deviceContext->Unmap(m_pMatrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	deviceContext->PSSetShaderResources(0, 1, &colorTexture);
	deviceContext->PSSetShaderResources(1, 1, &normalTexture);
	deviceContext->PSSetShaderResources(2, 1, &depthTexture);

	const Vec4* dir = pScene->GetLightingManager()->GetLightDirection();

	result = deviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (LightBufferType*)mappedResource.pData;
	dataPtr2->lightDirection = Vec4(dir->x, dir->y, dir->z, 1);

	deviceContext->Unmap(m_pLightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_pLightBuffer);

	result = deviceContext->Map(m_pCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr3 = (beShading::CameraBufferType*)mappedResource.pData;
	dataPtr3->cameraPosition = pScene->GetCamera()->GetPosition();
	dataPtr3->padding = 1;

	deviceContext->Unmap(m_pCameraBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &m_pCameraBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &m_pCameraBuffer);

	return true;
}

void DeferredLightShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(m_pLayout);

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &m_pPointSampleState);
	deviceContext->PSSetSamplers(1, 1, &m_pDepthSampleState);

	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}