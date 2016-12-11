#include "Actor.h"

Actor::Actor(ActorId id)
{
	m_id = id;
	m_type = "Unknown";
}

Actor::~Actor()
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	m_components.clear();

	BE_ASSERT(m_components.empty());
}

bool Actor::Initialize(rapidxml::xml_node<>* pData)
{
	m_type = pData->first_attribute("type")->value();
	return true;
}

void Actor::PostInitialize()
{
	for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); it++)
	{
		it->second->VPostInit();
	}
}

void Actor::Update(const float deltaMs)
{
	for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->VUpdate(deltaMs);
	}
}

std::string Actor::ToXML()
{
	rapidxml::xml_document<> outDoc;
	
	rapidxml::xml_node<>* pActorElement = outDoc.allocate_node(rapidxml::node_element, "Actor");
	pActorElement->append_attribute(outDoc.allocate_attribute("type", m_type.c_str()));

	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		ActorComponent* pComponent = it->second;
		pActorElement->append_node(pComponent->VGenerateXml(outDoc));
	}

	outDoc.append_node(pActorElement);
	std::string outString;
	rapidxml::print(std::back_inserter(outString), outDoc);

	return outString;
}

void Actor::AddComponent(ActorComponent* pComponent)
{
	std::pair<ActorComponents::iterator, bool> success = m_components.insert(std::make_pair(pComponent->VGetId(), pComponent));
	BE_ASSERT(success.second);
}