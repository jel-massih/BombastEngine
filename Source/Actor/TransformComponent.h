#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../Graphics3D/Geometry.h"

//Simple representation of actor. Does not register with any physics or anything
class TransformComponent : public ActorComponent
{
public:
	static const char* g_Name;
	virtual const char* VGetName() const { return TransformComponent::g_Name; }

	TransformComponent() {}
	virtual bool VInitialize(rapidxml::xml_node<>* pData) override;
	virtual rapidxml::xml_node<>* VGenerateXml() override;

	Mat4x4 GetTransform() const { return m_transform; }
	void SetTransform(const Mat4x4& newTransform){ m_position = newTransform.GetPosition(); m_rotation = newTransform.GetYawPitchRoll(); UpdateTransform(); }
	Vec3 GetPosition() const{ return m_position; }
	void SetPosition(const Vec3& pos) { m_position = pos; UpdateTransform(); }
	Vec3 GetRotation() const{ return m_rotation; }
	void SetRotation(const Vec3& rot) { m_rotation = rot; UpdateTransform(); }
	Vec3 GetLookAt() const { return m_transform.GetDirection(); }

	void MoveForward(float magnitude);
	void MoveUp(float magnitude);
	void MoveLeft(float magnitude);
	void RotateRight(float magnitude);
	void RotateUp(float magnitude);
private:
	void UpdateTransform();

private:
	Vec3 m_position;
	Vec3 m_rotation;

	Mat4x4 m_transform;
};

#endif