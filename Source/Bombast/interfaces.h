#ifndef INTERFACES_H
#define INTERFACES_H

#include "..\Utilities\rapidxml.hpp"

class Actor;

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

class IGameLogic
{
public:
	virtual Actor* VGetActor(const ActorId id) = 0;
	virtual Actor* VCreateActor(const std::string &actorResource, rapidxml::xml_node<> *overrides) = 0;
	virtual void VDestroyActor(const ActorId actorId) = 0;
	virtual bool VLoadGame(const char* levelResource) = 0;
	virtual void VChangeState(enum CoreGameState newState) = 0;
};

#endif