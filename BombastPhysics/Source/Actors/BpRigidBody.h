#pragma once

#include "BpRigidActor.h"

namespace bPhysics
{
	class BpRigidBody : public BpRigidActor
	{
	public:

		inline bool IsInIsland() const { return m_inIsland; }
		void SetInIsland(bool bIsland) { m_inIsland = bIsland; }

	protected:
		inline BpRigidBody() {}
		virtual ~BpRigidBody() {}

	private:
		bool m_inIsland;
	};
}