#pragma once

#include "BpGeometry.h"
#include "../Actors/BpRigidActor.h"

namespace bPhysics
{
	class BpShape
	{
	public:
		BpShape(BpGeometry* geometry, bool isExclusive);

		inline const BpMat4x4& GetWorldTransform() const { return m_transform; }
		inline void SetWorldTransform(const BpMat4x4& t) { m_transform = t; }

		inline const BpGeometry& GetGeometry() const { return *m_geometry; }

	private:
		BpMat4x4 m_transform;
		BpGeometry* m_geometry;
	};
}