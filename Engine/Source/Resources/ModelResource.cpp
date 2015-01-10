#include "ModelResource.h"
#include "../Bombast/BombastApp.h"
#include <sstream>

class MaterialResourceReader :
	public tinyobj::MaterialReader
{
public:
	MaterialResourceReader(const std::string& mtl_basepath) : m_mtlBasePath(mtl_basepath) {}
	virtual ~MaterialResourceReader() {}
	virtual std::string operator() (
		const std::string& matId,
		std::vector<tinyobj::material_t>& materials,
		std::map<std::string, int>& matMap);

private:
	std::string m_mtlBasePath;
};

std::string MaterialResourceReader::operator() (
	const std::string& matId,
	std::vector<tinyobj::material_t>& materials,
	std::map<std::string, int>& matMap)
{
	std::string filepath;

	if (!m_mtlBasePath.empty()) {
		filepath = std::string(m_mtlBasePath) + matId;
	}
	else {
		filepath = matId;
	}

	Resource resource(filepath);
	ResourceHandle* pResourceHandle = g_pApp->m_pResourceCache->GetHandle(&resource);

	std::stringstream ss(pResourceHandle->Buffer());

	return tinyobj::LoadMtl(matMap, materials, ss);
}

void ModelResourceExtraData::Shutdown()
{
	SAFE_DELETE(m_pModelData);
}

ModelClass* ModelResourceExtraData::GetModel()
{
	return m_pModelData;
}

void TestSt(std::istream& is)
{

}

bool ModelResourceExtraData::LoadModel(char* pRawBuffer, unsigned int rawSize, std::string modelFilepath)
{
	SAFE_DELETE(m_pModelData);
	m_pModelData = BE_NEW ModelClass;
	std::istringstream ss(pRawBuffer);
	unsigned int slashIndex = modelFilepath.find_last_of("\\");
	MaterialResourceReader matResourceReader(modelFilepath.substr(0, slashIndex+1));
	std::string result = tinyobj::LoadObj(m_pModelData->shapes, m_pModelData->materials, ss, matResourceReader);
	if (!result.empty()) {
		BE_ERROR(result);
		return false;
	}

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