#include "XmlResource.h"
#include "../Bombast/BombastApp.h"

void XmlResourceExtraData::ParseXml(char* pRawBuffer)
{
	m_xmlDocument.parse<0>(pRawBuffer);
}


bool XmlResourceLoader::VLoadResource(char *rawBuffer, unsigned int rawSize, ResourceHandle* handle)
{
	if (rawSize <= 0)
		return false;

	XmlResourceExtraData* pExtraData = BE_NEW XmlResourceExtraData();
	pExtraData->ParseXml(rawBuffer);

	handle->SetExtra(pExtraData);

	return true;
}


IResourceLoader* CreateXmlResourceLoader()
{
	return BE_NEW XmlResourceLoader();
}


rapidxml::xml_node<>* XmlResourceLoader::LoadAndReturnRootXmlElement(const char* resourceString)
{
	Resource resource(resourceString);
	ResourceHandle* pResourceHandle = BombastApp::GetGameInstance()->m_pResourceCache->GetHandle(&resource);  // load xml file from zip
	XmlResourceExtraData* pExtraData = (XmlResourceExtraData*)(pResourceHandle->GetExtra());
	return pExtraData->GetRoot();
}


