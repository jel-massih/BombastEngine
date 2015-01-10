#pragma once

#include "Actor.h"
#include "ActorComponent.h"

class SceneNode;

class RenderComponentInterface : public ActorComponent
{
public:
	virtual SceneNode* VGetSceneNode() = 0;
};