#pragma once

#include <vector>

namespace bPhysics
{
	class BpContactConstraint;
	class BpRigidDynamic;

	class BpContactManager
	{
	public:
		BpContactManager();

		void AddContact(BpRigidDynamic* body1, BpRigidDynamic* body2);

		//Find Contacts Via. Broadphase
		void FindContacts();

		void RemoveContact(BpContactConstraint* contact);

		void RemoveContactsFromBody(BpRigidDynamic* body);

		//Remove Previous Collisions outside broadphase
		void CheckCollisions();

		void SolveCollision();

		void ClearContactIslandFlags();

	private:
		std::vector<BpContactConstraint*> m_contacts;

		BpBroadPhase m_broadphase;
	};

}