#include "StaticMeshShader.h"
#include "../Graphics3D/Material.h"
#include "../Graphics3D/Scene.h"
#include "ShadingCommon.h"

using namespace beShading;

const int PixelProgramIndices[] {
	0, //SMP_UNLIT_COLORED
	1, //SMP_UNLIT_TEXTURED

	2, //SMP_LIT_COLORED
	3 //SMP_LIT_TEXTURED
};

const char* PixelShaderPrograms[] =
{
	"EngineResources.Shaders.staticmesh_unlit_colored.cso", //SMP_UNLIT_COLORED
	"EngineResources.Shaders.staticmesh_unlit_textured.cso", //SMP_UNLIT_TEXTURED
	"EngineResources.Shaders.staticmesh_lit_colored.cso", //SMP_LIT_COLORED
	"EngineResources.Shaders.staticmesh_lit_textured.cso" //SMP_LIT_TEXTURED
};

const int DeferredPixelProgramIndices[] {
	0, //COLORED
	1, //TEXTURED
};

const char* DeferredPixelShaderPrograms[] =
{
	"EngineResources.Shaders.staticmesh_deferred_colored.cso", //SMP_LIT_COLORED
	"EngineResources.Shaders.staticmesh_deferred_textured.cso" //SMP_LIT_TEXTURED
};

const char* VertexShaderProgram = "EngineResources.Shaders.staticmesh_vertex.cso";
const char* DeferredVertexShaderProgram = "EngineResources.Shaders.staticmesh_deferred_vertex.cso";

//Must be same size of pixelshaderprograms
static_assert(sizeof(PixelShaderPrograms) / sizeof(char*) == sizeof(PixelProgramIndices) / sizeof(int), "Program Indices count must match Pixel program count");
static_assert(sizeof(DeferredPixelShaderPrograms) / sizeof(char*) == sizeof(DeferredPixelProgramIndices) / sizeof(int), "Program Indices count must match Deferred Pixel program count");

class StaticMeshShader::ProgramImpl
{
public:
	ID3D11PixelShader* m_loadedPixelPrograms[sizeof(PixelProgramIndices) / sizeof(int)];
	ID3D11PixelShader* m_loadedDeferredPixelPrograms[sizeof(DeferredPixelProgramIndices) / sizeof(int)];
	ID3D11VertexShader* m_pForwardVertexProgram;
	ID3D11VertexShader* m_pDeferredVertexProgram;
};

StaticMeshShader::StaticMeshShader()
	:m_pProgramImpl(new ProgramImpl())
{
	m_pForwardLayout = nullptr;
	m_pDeferredLayout = nullptr;
	m_pMatrixBuffer = nullptr;
	m_pCameraBuffer = nullptr;
	m_pLightBuffer = nullptr;
	m_pSampleState = nullptr;
}

StaticMeshShader::~StaticMeshShader()
{
	SAFE_RELEASE(m_pSampleState);
	SAFE_RELEASE(m_pForwardLayout);
	SAFE_RELEASE(m_pDeferredLayout);
	SAFE_RELEASE(m_pLightBuffer);
	SAFE_RELEASE(m_pMaterialBuffer);
	SAFE_RELEASE(m_pCameraBuffer);
	SAFE_RELEASE(m_pMatrixBuffer);

	SAFE_DELETE(m_pProgramImpl);
}

bool StaticMeshShader::Initialize(ID3D11Device* device)
{
	bool result;

	//Load all shaders

	result = InitializeShaders(device);
	if (!result)
	{
		return false;
	}

	return true;
}

bool StaticMeshShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, const Material* material, const Scene* pScene)
{
	bool result;

	if (material->GetShaderType() == BSHADER_TYPE_DEFERRED_LIT)
	{
		result = RenderDeferred(deviceContext, indexCount, material, pScene);
	}
	else
	{
		result = SetShaderParameters(deviceContext, indexCount, material, pScene);
	}

	if (!result)
	{
		return false;
	}

	return true;
}

bool StaticMeshShader::InitializeShaders(ID3D11Device* device)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC materialBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	//Load each pixel shader
	for (int i = 0; i < sizeof(PixelShaderPrograms) / sizeof(char*); i++)
	{
		Resource pixelShaderResource(PixelShaderPrograms[i]);
		ResourceHandle* pPixelResHandle = g_pApp->m_pResourceCache->GetHandle(&pixelShaderResource);

		result = device->CreatePixelShader(pPixelResHandle->Buffer(), pPixelResHandle->Size(), nullptr, &(m_pProgramImpl->m_loadedPixelPrograms[i]));
		if (FAILED(result))
		{
			BE_ERROR("Failed to create Pixel shader");

			return false;
		}
	}

	for (int i = 0; i < sizeof(DeferredPixelShaderPrograms) / sizeof(char*); i++)
	{
		Resource pixelShaderResource(DeferredPixelShaderPrograms[i]);
		ResourceHandle* pPixelResHandle = g_pApp->m_pResourceCache->GetHandle(&pixelShaderResource);

		result = device->CreatePixelShader(pPixelResHandle->Buffer(), pPixelResHandle->Size(), nullptr, &(m_pProgramImpl->m_loadedDeferredPixelPrograms[i]));
		if (FAILED(result))
		{
			BE_ERROR("Failed to create Deferred Pixel shader");

			return false;
		}
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

	{
		Resource vertexShaderResource(VertexShaderProgram);
		ResourceHandle* pVertexResHandle = g_pApp->m_pResourceCache->GetHandle(&vertexShaderResource);

		result = device->CreateVertexShader(pVertexResHandle->Buffer(), pVertexResHandle->Size(), nullptr, &(m_pProgramImpl->m_pForwardVertexProgram));
		if (FAILED(result))
		{
			BE_ERROR("Failed to create vertex shader");

			return false;
		}

		//Create vertex input layout
		result = device->CreateInputLayout(polygonLayout, numElements, pVertexResHandle->Buffer(), pVertexResHandle->Size(), &m_pForwardLayout);
		if (FAILED(result))
		{
			return false;
		}
	}

	{
		Resource vertexShaderResource(DeferredVertexShaderProgram);
		ResourceHandle* pVertexResHandle = g_pApp->m_pResourceCache->GetHandle(&vertexShaderResource);

		result = device->CreateVertexShader(pVertexResHandle->Buffer(), pVertexResHandle->Size(), nullptr, &(m_pProgramImpl->m_pDeferredVertexProgram));
		if (FAILED(result))
		{
			BE_ERROR("Failed to create Deferred vertex shader");

			return false;
		}

		//Create vertex input layout
		result = device->CreateInputLayout(polygonLayout, numElements, pVertexResHandle->Buffer(), pVertexResHandle->Size(), &m_pDeferredLayout);
		if (FAILED(result))
		{
			return false;
		}
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

bool StaticMeshShader::RenderDeferred(ID3D11DeviceContext* deviceContext, int indexCount, const Material* material, const Scene* pScene)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	MaterialBufferType* dataPtr2;

	bool bUseTexture = false;

	result = deviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	 dataPtr = (MatrixBufferType*)mappedResource.pData;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();

	Mat4x4 world, view, projection;
	pRenderer->VGetViewMatrix(view);
	pRenderer->VGetWorldMatrix(world);
	pRenderer->VGetProjectionMatrix(projection);

	dataPtr->world = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&world));
	dataPtr->view = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&view));
	dataPtr->projection = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&projection));

	deviceContext->Unmap(m_pMatrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);


	result = deviceContext->Map(m_pMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (MaterialBufferType*)mappedResource.pData;

	dataPtr2->ambient = material->GetAmbient();
	dataPtr2->diffuse = material->GetDiffuse();
	dataPtr2->emissive = material->GetEmissive();
	dataPtr2->padding = XMFLOAT3(0, 0, 0);
	Vec4 specular;
	material->GetSpecular(specular, dataPtr2->specularPower);
	dataPtr2->specular = specular;

	//if material has any textures, then use first one
	if (material->GetTextures().size() > 0) {
		bUseTexture = true;
		ID3D11ShaderResourceView* texture = material->GetTextures().front()->GetTexture();
		deviceContext->PSSetShaderResources(0, 1, &texture);
		deviceContext->PSSetSamplers(0, 1, &m_pSampleState);
	}

	deviceContext->Unmap(m_pMaterialBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_pMaterialBuffer);

	deviceContext->IASetInputLayout(m_pDeferredLayout);

	int programIndex = 0;

	if (bUseTexture) {
		programIndex++;
	}

 	RenderShader(deviceContext, indexCount, m_pProgramImpl->m_loadedDeferredPixelPrograms[programIndex], m_pProgramImpl->m_pDeferredVertexProgram);

	return true;
}

bool StaticMeshShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, int indexCount, const Material* material, const Scene* pScene)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	CameraBufferType* dataPtr2;
	MaterialBufferType* dataPtr3;
	LightBufferType* dataPtr4;
	unsigned int bufferNumber;


	bool bUseTexture = false;
	bool bUseLight = true;

	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();

	Mat4x4 world, view, projection;
	pRenderer->VGetViewMatrix(view);
	pRenderer->VGetWorldMatrix(world);
	pRenderer->VGetProjectionMatrix(projection);

	result = deviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&world));
	dataPtr->view = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&view));
	dataPtr->projection = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&projection));

	deviceContext->Unmap(m_pMatrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	result = deviceContext->Map(m_pCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (CameraBufferType*)mappedResource.pData;

	dataPtr2->cameraPosition = pScene->GetCamera()->GetPosition();
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
	dataPtr3->padding = XMFLOAT3(0, 0, 0);
	Vec4 specular;
	material->GetSpecular(specular, dataPtr3->specularPower);
	dataPtr3->specular = specular;

	//if material has any textures, then use first one
	if (material->GetTextures().size() > 0) {
		bUseTexture = true;
		ID3D11ShaderResourceView* texture = material->GetTextures().front()->GetTexture();
		deviceContext->PSSetShaderResources(0, 1, &texture);
		deviceContext->PSSetSamplers(0, 1, &m_pSampleState);
	}

	deviceContext->Unmap(m_pMaterialBuffer, 0);

	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pMaterialBuffer);

	result = deviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr4 = (LightBufferType*)mappedResource.pData;
	dataPtr4->eyePosition = Vec4(pScene->GetCamera()->GetPosition(), 1.0f);
	dataPtr4->globalAmbient = *pScene->GetLightingManager()->GetGlobalAmbient();

	const Vec4* lightDiffuse = pScene->GetLightingManager()->GetLightDiffuse();
	const Vec4* lightDirection = pScene->GetLightingManager()->GetLightDirection();

	dataPtr4->light.enabled = true;
	dataPtr4->light.lightColor = lightDiffuse[0];
	Vec4 dir = lightDirection[0];
	dataPtr4->light.lightDirection = Vec3(dir.x, dir.y, dir.z);

	deviceContext->Unmap(m_pLightBuffer, 0);
	bufferNumber = 1;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pLightBuffer);

	int programIndex = 0;

	if (bUseTexture) {
		programIndex += 1;
	}

	if (bUseLight) {
		programIndex += 2;
	}

	deviceContext->IASetInputLayout(m_pForwardLayout);

	RenderShader(deviceContext, indexCount, m_pProgramImpl->m_loadedPixelPrograms[programIndex], m_pProgramImpl->m_pForwardVertexProgram);

	return true;
}

void StaticMeshShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount, ID3D11PixelShader* program, ID3D11VertexShader* vertexProgram)
{
	deviceContext->VSSetShader(vertexProgram, NULL, 0);
	deviceContext->PSSetShader(program, NULL, 0);

	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}