#ifndef XML_RESOURCE_H
#define XML_RESOURCE_H

#include "ResourceCache.h"

class XmlResourceExtraData : public IResourceExtraData
{
	rapidxml::xml_document<> m_xmlDocument;

public:
	virtual std::string VToString() { return "XmlResourceExtraData"; }
	void ParseXml(char* pRawBuffer);
	rapidxml::xml_node<>* GetRoot(void) { return m_xmlDocument.first_node(); }
};


//Loads xml files from resource Cache
class XmlResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) { return rawSize; }
	virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, ResourceHandle* handle);
	virtual std::string VGetPattern() { return "*.xml"; }

	static rapidxml::xml_node<>* LoadAndReturnRootXmlElement(const char* resourceString);
};

#endif