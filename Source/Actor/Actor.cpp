#include "Actor.h"

Actor::Actor(ActorId id)
{
	m_id = id;
	m_type = "";
}

Actor::~Actor()
{

}

bool Actor::Initialize()
{
	return false;
}

void Actor::Shutdown()
{

}

void Actor::AddComponent(ActorComponent* pComponent)
{

}

std::string Actor::ToXML()
{
	return "";
}