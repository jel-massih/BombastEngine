#include "XmlResource.h"
#include "../Bombast/BombastApp.h"

void XmlResourceExtraData::Shutdown()
{
	SAFE_DELETE_ARRAY(m_pRawBuffer);
	m_xmlDocument.clear();
}

void XmlResourceExtraData::ParseXml(char* pRawBuffer)
{
	m_xmlDocument.parse<0>(pRawBuffer);
	m_pRawBuffer = pRawBuffer;
}

rapidxml::xml_node<>* XmlResourceExtraData::GetRoot() 
{ 
	return m_xmlDocument.first_node(); 
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
	ResourceHandle* pResourceHandle = g_pApp->m_pResourceCache->GetHandle(&resource);  // load xml file from zip
	XmlResourceExtraData* pExtraData = (XmlResourceExtraData*)(pResourceHandle->GetExtra());
	rapidxml::xml_node<>* node = pExtraData->GetRoot();
	return node;
}