#pragma once

extern IGamePhysics* CreateGamePhysics();

struct PhysicsMaterialData
{
	float m_restitution;
	float m_friction;

	PhysicsMaterialData(float restitution, float friction)
	{
		m_restitution = restitution;
		m_friction = friction;
	}

	PhysicsMaterialData(const PhysicsMaterialData& other)
	{
		m_restitution = other.m_restitution;
		m_friction = other.m_friction;
	}
};
