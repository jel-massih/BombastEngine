#pragma once

#include "ActorComponent.h"

class PhysicsComponent : public ActorComponent
{
public:
	const static char* g_Name;
	virtual const char* VGetName() const override{ return PhysicsComponent::g_Name; }

	PhysicsComponent();
	~PhysicsComponent();
	
	virtual bool VInitialize(rapidxml::xml_node<>* pData) override;
	virtual void VPostInit() override;
	virtual void VUpdate(int deltaMs) override;
	virtual rapidxml::xml_node<>* VGenerateXml() override { return NULL; }

	//Physics
	void ApplyForce(const Vec3& direction, float newtons);
	void ApplyTorque(const Vec3& direction, float newtons);
	bool KMove(const Mat4x4& transform);

	//Acceleration
	void ApplyAcceleration(float accel);
	void RemoveAcceleration();
	void ApplyAngularAcceleration(float accel);
	void RemoveAngularAcceleration();

	Vec3 GetVelocity();
	void SetVelocity(const Vec3& velocity);
	void RotateY(float angleRadians);
	void SetPosition(float x, float y, float z);
	void Stop();

protected:
	void CreateShape();
	void BuildTransform(rapidxml::xml_node<>* pTransformElement);

	float m_acceleration, m_angularAcceleration;
	float m_maxVelocity, m_maxAngularVelocity;
	float m_linearDamping, m_angularDamping;

	std::string m_shape;
	std::string m_density;
	std::string m_material;

	bool m_bGravityEnabled;
	
	Vec3 m_location; //offset from actor
	Vec3 m_orientation; //Offset from actor
	Vec3 m_scale; //Offset from actor

	IGamePhysics* m_pGamePhysics;
};