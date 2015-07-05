#include "BpContactManager.h"
#include "BpContact.h"
#include "../Actors/BpRigidDynamic.h"
#include "../Geometry/BpShape.h"

using namespace bPhysics;

BpContactManager::BpContactManager()
{
}

void BpContactManager::AddContact(BpRigidDynamic* body1, BpRigidDynamic* body2)
{
	//Check fo existing contacts, return if dpe
	for (auto it = body1->GetShape()->GetContacts().begin(); it != body2->GetShape()->GetContacts().end(); it++)
	{
		if ((*it)->other == body2)
		{
			if ((*it)->constraint->shape1 == body1->GetShape() && (*it)->constraint->shape2 == body2->GetShape())
			{
				return;
			}
		}
	}

	//Create constraint
	BpContactConstraint* contact = BP_NEW BpContactConstraint();
	contact->shape1 = body1->GetShape();
	contact->shape2 = body2->GetShape();
	contact->manifold.SetPair(body1->GetShape(), body2->GetShape());
	contact->manifold.contactCount = 0;

	for (int i = 0; i < 8; i++)
	{
		contact->manifold.contacts[i].warmStarted = 0;
	}

	m_contacts.push_back(contact);

	//Connect A
	contact->edge1.constraint = contact;
	contact->edge1.other = body2;

	body1->AddContact(&contact->edge1);

	//Connect B
	contact->edge2.constraint = contact;
	contact->edge2.other = body2;

	body2->AddContact(&contact->edge2);

	body1->WakeUp();
	body2->WakeUp();
}

void BpContactManager::FindContacts()
{

}