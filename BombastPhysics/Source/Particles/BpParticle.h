#pragma once

#include "../Foundation/BpVec.h"

namespace bPhysics
{
		
	class BpParticle
	{
	public:
		void SetMass(f32 mass);
		void SetInverseMass(f32 inverseMass);

	private:
		BpVec3 m_position;
		BpVec3 m_velocity;
		BpVec3 m_acceleration;

		f32 m_damping;
		f32 m_inverseMass;
	};

}