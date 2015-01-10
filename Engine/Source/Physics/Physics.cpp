#include "Physics.h"
#include "PhysXPhysics.h"

IGamePhysics* CreateGamePhysics()
{
	IGamePhysics* gamePhysics;
	gamePhysics = BE_NEW PhysXPhysics;

	if (gamePhysics && !gamePhysics->VInitialize())
	{
		SAFE_DELETE(gamePhysics);
	}

	return gamePhysics;
}