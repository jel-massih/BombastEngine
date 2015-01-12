#include "ModelResource.h"
#include "../Bombast/BombastApp.h"
#include "../Utilities/BMDLMeshLoader.h"

void ModelResourceExtraData::Shutdown()
{
	SAFE_DELETE(m_pModelData);
}

ModelClass* ModelResourceExtraData::GetModel()
{
	return m_pModelData;
}

bool ModelResourceExtraData::LoadModel(char* pRawBuffer, unsigned int rawSize, std::string modelFilepath)
{
	m_pModelData = BE_NEW ModelClass;

	LoadMesh(&m_pModelData, reinterpret_cast<byte*>(pRawBuffer));

	return true;
}

bool ModelResourceLoader::VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle)
{
	bool result;

	if (rawSize <= 0)
	{
		return false;
	}

	ModelResourceExtraData* pExtraData = BE_NEW ModelResourceExtraData();
	result = pExtraData->LoadModel(rawBuffer, rawSize, handle->GetName());

	if (!result) {
		return false;
	}

	handle->SetExtra(pExtraData);

	return true;
}

IResourceLoader* CreateModelResourceLoader()
{
	return BE_NEW ModelResourceLoader();
}

ModelClass* ModelResourceLoader::LoadAndReturnModelResource(const char* resourceString)
{
	Resource resource(resourceString);
	ResourceHandle* pResourceHandle = g_pApp->m_pResourceCache->GetHandle(&resource);
	ModelResourceExtraData* pExtraData = (ModelResourceExtraData*)(pResourceHandle->GetExtra());

	ModelClass* model = pExtraData->GetModel();
	return model;
}