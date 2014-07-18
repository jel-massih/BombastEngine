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
	SceneNodeProperties();
	const ActorId& GetActorId() const { return m_actorId; }
	Mat4x4 const& ToWorld() const { return m_toWorld; }
	Mat4x4 const& FromWorld() const { return m_fromWorld; }
	void Transform(Mat4x4* toWorld, Mat4x4* fromWorld) const;

	const char* GetName() const { return m_name.c_str(); }

	bool HasAlpha() const { return m_name.c_str(); }
	float GetAlpha() const { return m_material.GetAlpha(); }
	AlphaType GetAlphaType() const { return m_alphaType; }

	RenderPass GetRenderPass() const { return m_renderPass; }
	float GetRadius() const { return m_radius; }

	Material GetMaterial() const { return m_material; }

protected:
	ActorId m_actorId;
	std::string m_name;
	Mat4x4 m_toWorld, m_fromWorld;
	float m_radius;
	RenderPass m_renderPass;
	Material m_material;
	AlphaType m_alphaType;

	void SetAlpha(const float alpha) { m_alphaType = AlphaMaterial; m_material.SetAlpha(alpha); }
};

#endif