#pragma once

#include "../Bombast/interfaces.h"
#include <map>

enum EPhysicsEngine
{
	BE_PHYSICS_STUB,
	BE_PHYSICS_BOMBAST,
	BE_PHYSICS_PHYSX
};

extern IGamePhysics* CreateGamePhysics(EPhysicsEngine engineType);

extern std::map<std::string, EPhysicsEngine> StringToPhysicsEngineType;

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
