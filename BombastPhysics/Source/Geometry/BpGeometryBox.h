#pragma once

#include "../Foundation/BpMath.h"
#include "BpGeometry.h"

namespace bPhysics
{
	class BpGeometryBox : public BpGeometry
	{
	public:
		inline BpGeometryBox() : BpGeometry(BpGeometryType::BOX), eX(0), eY(0), eZ(0) {}
		inline BpGeometryBox(f32 extentX, f32 extentY, f32 extentZ) : BpGeometry(BpGeometryType::BOX), eX(extentX), eY(extentY), eZ(extentZ) {}

		//Returns True if Geometry is Valid. A valid Box has ex, ey and ez > 0
		inline bool IsValid() const;

	public:
		f32 eX, eY, eZ;
	};

	inline bool BpGeometryBox::IsValid() const
	{
		if (m_type != BpGeometryType::BOX)
		{
			return false;
		}
		if (!BpIsFinite(eX) || !BpIsFinite(eY) || !BpIsFinite(eZ))
		{
			return false;
		}
		if (eX <= 0.0f || eY <= 0.0f || eZ <= 0.0f)
		{
			return false;
		}

		return true;
	}
}