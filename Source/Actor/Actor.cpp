#include "Actor.h"

Actor::Actor(ActorId id)
{
	m_id = id;
	m_type = "";
}

Actor::~Actor()
{

}

void Actor::Shutdown()
{

}

std::string Actor::ToXML()
{
	return "";
}