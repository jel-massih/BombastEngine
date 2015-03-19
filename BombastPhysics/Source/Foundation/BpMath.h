#pragma once
#include <cmath>

namespace bPhysics
{
	//====== Misc Helpers ===========
	inline bool BpIsFinite(float f) { return isfinite(f); }

	//====== BpVec3 ===========
	class BpVec3
	{
	public:
		BpVec3()
			:x(0.f), y(0.f), z(0.f) {}

		BpVec3(float f)
			:x(f), y(f), z(f) {}

		BpVec3(float _x, float _y, float _z)
			:x(_x), y(_y), z(_z) {}

		inline BpVec3 operator*(float f) const;

		inline bool IsFinite() const {
			return BpIsFinite(x) && BpIsFinite(y) && BpIsFinite(z);
		}

		static const BpVec3 g_InvalidBpVec3;

	public:
		float x, y, z;
	};

	inline BpVec3 BpVec3::operator*(float f) const
	{
		return BpVec3(x * f, y * f, z * f);
	}

	inline BpVec3 operator *(const float f, const BpVec3 v)
	{
		return v * f;
	}

	//====== BpVec4 ===========
	class BpVec4
	{
	public:
		BpVec4()
			:x(0.f), y(0.f), z(0.f), w(0.f) {}

		BpVec4(float _x, float _y, float _z, float _w)
			:x(_x), y(_y), z(_z), w(_w) {}

		inline BpVec4 operator*(float f) const;

		inline bool IsFinite() const {
			return BpIsFinite(x) && BpIsFinite(y) && BpIsFinite(z) && BpIsFinite(w);
		}

		static const BpVec4 g_InvalidBpVec4;

	public:
		float x, y, z, w;
	};

	inline BpVec4 BpVec4::operator*(float f) const
	{
		return BpVec4(x * f, y * f, z * f, w * f);
	}

	inline BpVec4 operator *(const float f, const BpVec4 v)
	{
		return v * f;
	}

	//====== BpMat4x4 ===========
	class BpMat4x4
	{
	public:
		inline bool IsValid() const
		{
			return col0.IsFinite() && col1.IsFinite() && col2.IsFinite() && col3.IsFinite();
		}

		BpVec4 col0, col1, col2, col3;
	};
}