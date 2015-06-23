#pragma once

#include "../Foundation/BpMath.h"
#include "BpGeometry.h"

namespace bPhysics
{
	class BpGeometrySphere : public BpGeometry
	{
	public:
		inline BpGeometrySphere() : BpGeometry(BpGeometryType::SPHERE), radius(0) {}
		inline BpGeometrySphere(float r) : BpGeometry(BpGeometryType::SPHERE), radius(r) {}

		BpDebugShape* CreateDebugShape() const override;

		//Returns True if Geometry is Valid. A valid sphere has radius > 0
		inline bool IsValid() const;

	public:
		float radius;
	};

	inline bool BpGeometrySphere::IsValid() const
	{
		if (m_type != BpGeometryType::SPHERE)
		{
			return false;
		}
		if (!BpIsFinite(radius))
		{
			return false;
		}
		if (radius <= 0.0f)
		{
			return false;
		}

		return true;
	}
}