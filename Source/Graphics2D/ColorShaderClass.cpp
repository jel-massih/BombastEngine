#include "colorshaderclass.h"
#include "../Bombast/BombastApp.h"


ColorShaderClass::ColorShaderClass()
{
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pLayout = 0;
	m_pMatrixBuffer = 0;
}


ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
{
}


ColorShaderClass::~ColorShaderClass()
{
}


bool ColorShaderClass::Initialize(ID3D11Device* device)
{
	bool result;

	result = InitializeShader(device, L"../../Game/Data/color.vs", L"../../Game/Data/color.ps");
	if (!result)
	{
		return false;
	}

	return true;
}


void ColorShaderClass::Shutdown()
{
	ShutdownShader();
}


bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX &proj)
{
	bool result;

	result = SetShaderParameters(deviceContext, world, view, proj);
	if (!result)
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}


bool ColorShaderClass::InitializeShader(ID3D11Device* device, WCHAR* vertexShaderPath, WCHAR* pixelShaderPath)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DCompileFromFile(vertexShaderPath, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			BE_ERROR(L"Messed Up Vertex Shader File");
		}
		else
		{
			BE_ERROR(L"Missing Vertex Shader File");
		}

		return false;
	}

	result = D3DCompileFromFile(pixelShaderPath, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			BE_ERROR(L"Messed Up Pixel Shader File");
		}
		else
		{
			BE_ERROR(L"Missing Vertex Shader File");
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

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

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

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void ColorShaderClass::ShutdownShader()
{
	SAFE_RELEASE(m_pMatrixBuffer);
	SAFE_RELEASE(m_pLayout);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);
}


bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* context, DirectX::XMMATRIX &world, DirectX::XMMATRIX &view, DirectX::XMMATRIX &proj)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;


	world = DirectX::XMMatrixTranspose(world);
	view = DirectX::XMMatrixTranspose(view);
	proj = DirectX::XMMatrixTranspose(proj);

	result = context->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = proj;

	context->Unmap(m_pMatrixBuffer, 0);

	bufferNumber = 0;

	context->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	return true;
}


void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(m_pLayout);

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}