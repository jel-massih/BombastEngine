#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../Graphics3D/Geometry.h"

//Simple representation of actor. Does not register with any physics or anything
class TransformComponent : public ActorComponent
{
	Mat4x4 m_transform;

public:
	static const char* g_Name;
	virtual const char* VGetName() const { return g_Name; }

	TransformComponent() : m_transform(Mat4x4::g_Identity){}
	virtual bool VInitialize(rapidxml::xml_node<>* pData) override;
	virtual rapidxml::xml_node<>* VGenerateXml() override;

	Mat4x4 GetTransform() const { return m_transform; }
	void SetTransform(const Mat4x4& newTransform){ m_transform = newTransform; }
	Vec3 GetPosition() const{ return m_transform.GetPosition(); }
	void SetPosition(const Vec3& pos) { m_transform.SetPosition(pos); }
	Vec3 GetLookAt() const { return m_transform.GetDirection(); }
};

#endif