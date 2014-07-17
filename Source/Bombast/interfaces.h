#ifndef INTERFACES_H
#define INTERFACES_H

#include "..\Utilities\rapidxml.hpp"
#include <string>

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


class ResourceHandle;
class Resource;

class IResourceLoader
{
public:
	virtual std::string VGetPattern() = 0;
	virtual bool VUseRawFile() = 0;
	virtual bool VDiscardRawBufferAfterLoad() = 0;
	virtual bool VAddNullZero() { return false; }
	virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) = 0;
	virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, ResourceHandle* handle) = 0;
};

class IResourceFile
{
public:
	virtual bool VOpen() = 0;
	virtual int VGetRawResourceSize(const Resource &r) = 0;
	virtual int VGetRawResource(const Resource &r, char *buffer) = 0;
	virtual int VGetNumResources() const = 0;
	virtual std::string VGetResourceName(int num) const = 0;
	virtual ~IResourceFile() { }
};

enum RenderPass
{
	RenderPass_0,
	RenderPass_Static = RenderPass_0,
	RenderPass_Actor,
	RenderPass_Sky,
	RenderPass_NotRendered,
	RenderPass_Last
};

#endif