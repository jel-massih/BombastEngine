#ifndef RENDER_COMPONENT_INTERFACE
#define RENDER_COMPONENT_INTERFACE

#include "ActorComponent.h"

class RenderComponentInterface : public ActorComponent
{
public:
	virtual SceneNode* VGetSceneNode() = 0;
};

#endif