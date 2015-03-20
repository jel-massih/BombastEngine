#pragma once

#include "BpGeometry.h"
#include "BpGeometrySphere.h"
#include "../Actors/BpRigidActor.h"

namespace bPhysics
{
	class BpShape
	{
	public:
		BpShape(const BpGeometry& geometry,
			bool isExclusive);

		inline const BpMat4x4& GetWorldTransform() const { return m_transform; }
		inline void SetWorldTransform(const BpMat4x4& t) { m_transform = t; }

	private:
		BpMat4x4 m_transform;
	};
}