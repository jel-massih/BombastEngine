#include "MaterialResource.h"

void MaterialResourceExtraData::Shutdown()
{
	SAFE_DELETE(m_pMaterial);
}

Material* MaterialResourceExtraData::GetMaterial()
{
	return m_pMaterial;
}

bool MaterialResourceExtraData::LoadMaterial(char* pRawBuffer, unsigned int rawSize)
{
	rapidxml::xml_document<> pDoc;

	m_pMaterial = BE_NEW Material;

	pDoc.parse<0>(pRawBuffer);

	rapidxml::xml_node<>* pAttribute = pDoc.first_node("Ambient");
	if (pAttribute)
	{
		Color c;
		c.r = (float)atof(pAttribute->first_attribute("r")->value());
		c.g = (float)atof(pAttribute->first_attribute("g")->value());
		c.b = (float)atof(pAttribute->first_attribute("b")->value());
		c.a = (float)atof(pAttribute->first_attribute("a")->value());
		m_pMaterial->SetAmbient(c);
	}

	pAttribute = pDoc.first_node("BaseDiffuse");
	if (pAttribute)
	{
		Color c;
		c.r = (float)atof(pAttribute->first_attribute("r")->value());
		c.g = (float)atof(pAttribute->first_attribute("g")->value());
		c.b = (float)atof(pAttribute->first_attribute("b")->value());
		c.a = (float)atof(pAttribute->first_attribute("a")->value());
		m_pMaterial->SetDiffuse(c);
	}

	pAttribute = pDoc.first_node("Specular");
	if (pAttribute)
	{
		float power;
		Color c;
		c.r = (float)atof(pAttribute->first_attribute("r")->value());
		c.g = (float)atof(pAttribute->first_attribute("g")->value());
		c.b = (float)atof(pAttribute->first_attribute("b")->value());
		c.a = (float)atof(pAttribute->first_attribute("a")->value());
		power = (float)atof(pAttribute->first_attribute("power")->value());
		m_pMaterial->SetSpecular(c, power);
	}

	pAttribute = pDoc.first_node("Emmisive");
	if (pAttribute)
	{
		Color c;
		c.r = (float)atof(pAttribute->first_attribute("r")->value());
		c.g = (float)atof(pAttribute->first_attribute("g")->value());
		c.b = (float)atof(pAttribute->first_attribute("b")->value());
		c.a = (float)atof(pAttribute->first_attribute("a")->value());
		m_pMaterial->SetEmissive(c);
	}

	return true;
}

bool MaterialResourceLoader::VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle)
{
	bool result;

	if (rawSize <= 0)
	{
		return false;
	}

	MaterialResourceExtraData* pExtraData = BE_NEW MaterialResourceExtraData();
	result = pExtraData->LoadMaterial(rawBuffer, rawSize);

	if (!result) {
		return false;
	}

	handle->SetExtra(pExtraData);

	return true;
}

IResourceLoader* CreateMaterialResourceLoader()
{
	return BE_NEW MaterialResourceLoader();
}

Material* MaterialResourceLoader::LoadAndReturnMaterialResource(const char* resourceString)
{
	Resource resource(resourceString);
	ResourceHandle* pResourceHandle = g_pApp->m_pResourceCache->GetHandle(&resource);
	MaterialResourceExtraData* pExtraData = (MaterialResourceExtraData*)(pResourceHandle->GetExtra());

	Material* material = pExtraData->GetMaterial();
	return material;
}