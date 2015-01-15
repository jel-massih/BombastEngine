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
		Vec4 c;
		c.x = (float)atof(pAttribute->first_attribute("r")->value());
		c.y = (float)atof(pAttribute->first_attribute("g")->value());
		c.z = (float)atof(pAttribute->first_attribute("b")->value());
		c.w = (float)atof(pAttribute->first_attribute("a")->value());
		m_pMaterial->SetAmbient(c);
	}

	pAttribute = pDoc.first_node("BaseDiffuse");
	if (pAttribute)
	{
		Vec4 c;
		c.x = (float)atof(pAttribute->first_attribute("r")->value());
		c.y = (float)atof(pAttribute->first_attribute("g")->value());
		c.z = (float)atof(pAttribute->first_attribute("b")->value());
		c.w = (float)atof(pAttribute->first_attribute("a")->value());
		m_pMaterial->SetDiffuse(c);
	}

	pAttribute = pDoc.first_node("Specular");
	if (pAttribute)
	{
		float power;
		Vec4 c;
		c.x = (float)atof(pAttribute->first_attribute("r")->value());
		c.y = (float)atof(pAttribute->first_attribute("g")->value());
		c.z = (float)atof(pAttribute->first_attribute("b")->value());
		c.w = (float)atof(pAttribute->first_attribute("a")->value());
		power = (float)atof(pAttribute->first_attribute("power")->value());
		m_pMaterial->SetSpecular(c, power);
	}

	pAttribute = pDoc.first_node("Emmisive");
	if (pAttribute)
	{
		Vec4 c;
		c.x = (float)atof(pAttribute->first_attribute("r")->value());
		c.y = (float)atof(pAttribute->first_attribute("g")->value());
		c.z = (float)atof(pAttribute->first_attribute("b")->value());
		c.w = (float)atof(pAttribute->first_attribute("a")->value());
		m_pMaterial->SetEmissive(c);
	}

	//Iterate over all textures and add to material
	pAttribute = pDoc.first_node("Textures");
	if (pAttribute)
	{
		for (rapidxml::xml_node<> *child = pAttribute->first_node(); child; child = child->next_sibling())
		{
			if (child->first_attribute("path") != NULL)
			{
				 m_pMaterial->AddTexture(child->first_attribute("path")->value());
			}
			else
			{
				BE_ERROR("Texture path Exception: Make sure path attribute is set for texture Element");
			}
		}
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