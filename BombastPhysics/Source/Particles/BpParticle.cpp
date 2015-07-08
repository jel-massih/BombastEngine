#include "BpParticle.h"

#include <assert.h>

using namespace bPhysics;

void BpParticle::SetMass(f32 mass)
{
	assert(mass != 0);
	m_inverseMass = (f32)1.0 / mass;
}

void BpParticle::SetInverseMass(f32 inverseMass)
{
	m_inverseMass = inverseMass;
}