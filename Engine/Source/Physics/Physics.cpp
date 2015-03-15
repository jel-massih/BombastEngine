#include "Physics.h"
#include "PhysXPhysics.h"
#include "BombastPhysics.h"

IGamePhysics* CreateGamePhysics(EPhysicsEngine engineType)
{
	IGamePhysics* gamePhysics;

	switch (engineType) {
	case EPhysicsEngine::BE_PHYSICS_PHYSX:
		gamePhysics = BE_NEW PhysXPhysics;
		break;
	case EPhysicsEngine::BE_PHYSICS_BOMBAST:
		gamePhysics = BE_NEW BombastPhysics;
		break;
	default:
		BE_ERROR("Invalid Physics Engine Type %i used for CreateGamePhysics", engineType);
		return nullptr;
	}

	if (gamePhysics && !gamePhysics->VInitialize())
	{
		SAFE_DELETE(gamePhysics);
	}

	return gamePhysics;
}

std::map<std::string, EPhysicsEngine> StringToPhysicsEngineType =
{
	{ "physx", EPhysicsEngine::BE_PHYSICS_PHYSX },
	{ "bombast", EPhysicsEngine::BE_PHYSICS_BOMBAST }
};