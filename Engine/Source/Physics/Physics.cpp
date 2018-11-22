#include "Physics.h"
#include "StubPhysics.h"
#include "PhysXPhysics.h"
#include "BombastPhysics.h"

IGamePhysics* CreateGamePhysics(EPhysicsEngine engineType)
{
	IGamePhysics* gamePhysics;

	switch (engineType) {
	case EPhysicsEngine::BE_PHYSICS_STUB:
		gamePhysics = BE_NEW StubPhysics;
		break;
#ifdef USE_PHYSX
	case EPhysicsEngine::BE_PHYSICS_PHYSX:
		gamePhysics = BE_NEW PhysXPhysics;
		break;
#endif
#ifdef USE_BOMBASTPHYSICS
	case EPhysicsEngine::BE_PHYSICS_BOMBAST:
		gamePhysics = BE_NEW BombastPhysics;
		break;
#endif
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
	{ "stub", EPhysicsEngine::BE_PHYSICS_STUB},
	{ "physx", EPhysicsEngine::BE_PHYSICS_PHYSX },
	{ "bombast", EPhysicsEngine::BE_PHYSICS_BOMBAST }
};