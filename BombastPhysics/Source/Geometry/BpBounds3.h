#pragma once

#include "../Foundation/BpTypes.h"
#include "../Foundation/BpAssert.h"
#include "../Foundation/BpVec.h"

//Max extents defined so that floating point exceptions are avoided for most use cases
#define BP_MAX_BOUNDS_EXTENTS (BP_MAX_REAL * 0.25f)

namespace bPhysics
{
	class BpBounds3
	{
	public:
		inline BpBounds3() {}
		inline BpBounds3(const BpVec3& min, const BpVec3& max);

		//Returns empty bounds
		static inline BpBounds3 Empty();

		//Set Bounds to Empty
		inline void SetEmpty();

		inline bool IsEmpty() const;

		inline bool IsFinite() const;
		inline bool IsValid() const;

		BpVec3 minimum, maximum;
	};

	inline BpBounds3::BpBounds3(const BpVec3& min, const BpVec3& max)
		: minimum(min), maximum(max)
	{}

	inline BpBounds3 BpBounds3::Empty()
	{
		return BpBounds3(BpVec3(BP_MAX_BOUNDS_EXTENTS), BpVec3(-BP_MAX_BOUNDS_EXTENTS));
	}

	inline bool BpBounds3::IsFinite() const
	{
		return minimum.IsFinite() && maximum.IsFinite();
	}

	inline void BpBounds3::SetEmpty()
	{
		minimum = BpVec3(BP_MAX_BOUNDS_EXTENTS);
		maximum = BpVec3(-BP_MAX_BOUNDS_EXTENTS);
	}

	inline bool BpBounds3::IsEmpty() const
	{
		BP_ASSERT(IsValid());
		return minimum.x > maximum.x;
	}

	inline bool BpBounds3::IsValid() const
	{
		return (IsFinite() &&
			(
				((minimum.x <= maximum.x) && (minimum.y <= maximum.y) && (minimum.z <= maximum.z)) ||
				(
					(minimum.x == BP_MAX_BOUNDS_EXTENTS) && (minimum.y == BP_MAX_BOUNDS_EXTENTS) && (minimum.z == BP_MAX_BOUNDS_EXTENTS) &&
					(maximum.x == -BP_MAX_BOUNDS_EXTENTS) && (maximum.y == -BP_MAX_BOUNDS_EXTENTS) && (maximum.z == -BP_MAX_BOUNDS_EXTENTS)
				)
			)
		);
	}
}