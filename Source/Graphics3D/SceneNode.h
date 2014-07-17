#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "../Actor/Actor.h"
#include "Geometry.h"
#include "Material.h"

class SceneNode;

//Defines available types of Alpha BLending
enum AlphaType
{
	AlphaOpaque,
	AlphaTexture,
	AlphaMaterial,
	AlphaVertex
};

//Contained in SceneNode class to provide easy accessor
class SceneNodeProperties
{
	friend class SceneNode;
public:

protected:
	ActorId m_actorId;
	std::string m_name;
	Mat4x4 m_toWorld, m_fromWorld;
	float m_radius;
	RenderPass m_renderPass;
	Material m_material;
	AlphaType m_alphaType;

};

#endif