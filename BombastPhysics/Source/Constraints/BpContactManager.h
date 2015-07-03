#pragma once

#include <vector>

namespace bPhysics
{
	class BpContactConstraint;
	class BpShape;

	class BpContactManager
	{
	public:
		BpContactManager();

		void AddContact(BpShape* shape1, BpShape* shape2);

		//Find Contacts Via. Broadphase
		void FindContacts();

		void RemoveContact(BpContactConstraint* contact);

		void RemoveContactsFromShape(BpShape* shape);

		//Remove Previous Collisions outside broadphase
		void CheckCollisions();

		void SolveCollision();

		void ClearContactIslandFlags();

	private:
		std::vector<BpContactConstraint*> m_contacts;
	};

}