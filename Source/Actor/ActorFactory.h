#ifndef ACTOR_FACTORY_H
#define ACTOR_FACTORY_H

#include "../Utilities/templates.h"

class ActorFactory
{
	ActorId m_lastActorId;

public:
	ActorFactory();

	Actor* CreateActor(const char* actorResource, rapidxml::xml_node<>* overrides);
	void ModifyActor(Actor* pActor, rapidxml::xml_node<>* overrides);

	virtual ActorComponent* VCreateComponent(rapidxml::xml_node<>* overrides);

protected:
	GenericObjectFactory<ActorComponent, ComponentId> m_componentFactory;

private:
	ActorId GetNextActorId() { m_lastActorId++; return m_lastActorId; }
};

#endif