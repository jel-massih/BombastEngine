#include "Actor.h"

Actor::Actor(ActorId id)
{
	m_id = id;
	m_type = "Unknown";
	m_resource = "Unknown";
}

Actor::~Actor()
{
	BE_ASSERT(m_components.empty());
}

bool Actor::Initialize(rapidxml::xml_node<>* pData)
{
	m_type = pData->first_attribute("type")->value();
	m_resource = pData->first_attribute("resource")->value();
	return true;
}

void Actor::Shutdown()
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	m_components.clear();
}

void Actor::Update(int deltaTime)
{
	for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->VUpdate(deltaTime);
	}
}

std::string Actor::ToXML()
{
	rapidxml::xml_document<> outDoc;
	
	rapidxml::xml_node<>* pActorElement = outDoc.allocate_node(rapidxml::node_element, "Actor");
	pActorElement->append_attribute(outDoc.allocate_attribute("type", m_type.c_str()));
	pActorElement->append_attribute(outDoc.allocate_attribute("resource", m_resource.c_str()));
	outDoc.append_node(pActorElement);

	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		ActorComponent* pComponent = it->second;
		rapidxml::xml_node<>* pComponentElement = pComponent->VGenerateXml();
		pActorElement->append_node(pComponentElement);
	}
	std::string outString;
	rapidxml::print(std::back_inserter(outString), outDoc);

	return outString;
}

void Actor::AddComponent(ActorComponent* pComponent)
{
	std::pair<ActorComponents::iterator, bool> success = m_components.insert(std::make_pair(pComponent->VGetId(), pComponent));
	BE_ASSERT(success.second);
}