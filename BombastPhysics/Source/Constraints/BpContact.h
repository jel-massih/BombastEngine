#pragma once

namespace bPhysics
{
	class BpShape;

	class BpContactConstraint
	{
	public:
		void SolveCollision();

		BpShape *a, *b;

		bool bColliding;
		bool bPrevColliding; //Contacts were colliding last step
		bool bInIsland; //Indicates if part of island
	};

}