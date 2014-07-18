#ifndef RENDER_COMPONENT_INTERFACE
#define RENDER_COMPONENT_INTERFACE

#include "Actor.h"
#include "ActorComponent.h"

class SceneNode;

class RenderComponentInterface : public ActorComponent
{
public:
	virtual SceneNode* VGetSceneNode() = 0;
};

#endif