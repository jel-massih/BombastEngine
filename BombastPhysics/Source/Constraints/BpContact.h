#pragma once

namespace bPhysics
{
	class BpRigidDynamic;

	struct BpContactEdge
	{
		BpRigidDynamic* other;
		BpContactConstraint* constraint;
	};

	union BpFeaturePair
	{
		struct
		{
			unsigned char inR;
			unsigned char outR;
			unsigned char inI;
			unsigned char outI;
		};

		int key;
	};

	struct BpContact
	{
		BpVec3 position;
		int penetrationDepth;
		int normalImpulse;
		int tangentImpulse[2];
		int bias; //restitution + baumgarte
		int normalMass;
		int tangentMass[2];
		BpFeaturePair featurePair;
		unsigned char warmStarted;
	};

	class BpManifold
	{
	public:
		void SetPair(BpShape* s1, BpShape* s2);

		BpShape shape1;
		BpShape shape2;

		BpVec3 normal; //shape1 -> shape2
		BpVec3 tangentVectors[2];
		BpContact contacts[8]; //1 each vert
		int contactCount;
	};

	class BpContactConstraint
	{
	public:
		void SolveCollision();

		BpShape *shape1, *shape2;

		BpContactEdge edge1;
		BpContactEdge edge2;

		BpManifold manifold;

		bool bColliding;
		bool bPrevColliding; //Contacts were colliding last step
		bool bInIsland; //Indicates if part of island
	};

}