#pragma once

#include "BpGeometry.h"
#include <cmath>

namespace bPhysics
{
	class BpGeometrySphere : public BpGeometry
	{
	public:
		inline BpGeometrySphere() : BpGeometry(BpGeometryType::SPHERE), radius(0) {}
		inline BpGeometrySphere(float r) : BpGeometry(BpGeometryType::SPHERE), radius(r) {}

		//Returns True if Geometry is Valid. A valid sphere has radius > 0
		inline bool IsValid() const;

	public:
		float radius;
	};

	inline bool BpGeometrySphere::isValid() const
	{
		if (m_type != BpGeometryType::SPHERE)
		{
			return false;
		}
		if (!isfinite(radius))
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