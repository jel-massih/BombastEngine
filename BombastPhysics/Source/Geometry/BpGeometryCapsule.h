#pragma once

#include "../Foundation/BpMath.h"
#include "BpGeometry.h"

namespace bPhysics
{
	class BpGeometryCapsule : public BpGeometry
	{
	public:
		inline BpGeometryCapsule() : BpGeometry(BpGeometryType::CAPSULE), radius(0), halfHeight(0) {}
		inline BpGeometryCapsule(f32 radius, f32 halfHeight) : BpGeometry(BpGeometryType::CAPSULE), radius(radius), halfHeight(halfHeight) {}

		//Returns True if Geometry is Valid. A valid capsule is radius > 0 and halfHeight > 0
		inline bool IsValid() const;

	public:
		f32 radius, halfHeight;
	};

	inline bool BpGeometryCapsule::IsValid() const
	{
		if (m_type != BpGeometryType::CAPSULE)
		{
			return false;
		}
		if (!BpIsFinite(radius) || !BpIsFinite(halfHeight))
		{
			return false;
		}
		if (radius <= 0.0f || halfHeight <= 0.0f)
		{
			return false;
		}

		return true;
	}
}