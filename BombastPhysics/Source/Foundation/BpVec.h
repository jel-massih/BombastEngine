#pragma once
#include "BpMath.h"
#include "BpSimpleTypes.h"
#include <limits>

namespace bPhysics
{

	//====== BpVec2 ===========
	class BpVec2
	{
	public:
		BpVec2()
			:x(0.f), y(0.f) {}

		BpVec2(f32 f)
			:x(f), y(f) {}

		BpVec2(f32 _x, f32 _y)
			:x(_x), y(_y) {}

		inline BpVec2 operator*(f32 f) const;

		inline BpVec2 operator+(BpVec2 v) const;
		inline BpVec2& operator+=(const BpVec2& v);

		inline bool IsFinite() const {
			return BpIsFinite(x) && BpIsFinite(y);
		}

		inline f32 Length() { return sqrt(x*x + y*y); }

	public:
		f32 x, y;
	};

	inline BpVec2 BpVec2::operator*(f32 f) const
	{
		return BpVec2(x * f, y * f);
	}

	inline BpVec2 operator *(const f32 f, const BpVec2 v)
	{
		return v * f;
	}

	inline BpVec2 BpVec2::operator+(BpVec2 v) const
	{
		return BpVec2(x + v.x, y + v.y);
	}

	inline BpVec2& BpVec2::operator+=(const BpVec2& v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	//====== BpVec3 ===========
	class BpVec3
	{
	public:
		BpVec3()
			:x(0.f), y(0.f), z(0.f) {}

		BpVec3(f32 f)
			:x(f), y(f), z(f) {}

		BpVec3(f32 _x, f32 _y, f32 _z)
			:x(_x), y(_y), z(_z) {}

		BpVec3(BpINVALID invalid)
			:x(-FLT_MAX), y(-FLT_MAX), z(-FLT_MAX) {}

		inline BpVec3 operator*(f32 f) const;

		inline BpVec3 operator+(BpVec3 v) const;
		inline BpVec3& operator+=(const BpVec3& v);

		inline bool IsFinite() const {
			return BpIsFinite(x) && BpIsFinite(y) && BpIsFinite(z);
		}

		inline f32 Length() { return sqrt(x*x + y*y + z*z); }

	public:
		f32 x, y, z;
	};

	inline BpVec3 BpVec3::operator*(f32 f) const
	{
		return BpVec3(x * f, y * f, z * f);
	}

	inline BpVec3 operator *(const f32 f, const BpVec3 v)
	{
		return v * f;
	}

	inline BpVec3 operator *(const BpVec3 v1, const BpVec3 v2)
	{
		return BpVec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
	}

	inline BpVec3 BpVec3::operator+(BpVec3 v) const
	{
		return BpVec3(x + v.x, y + v.y, z + v.z);
	}

	inline BpVec3& BpVec3::operator+=(const BpVec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	inline BpVec3 operator -(const BpVec3 v1, const BpVec3 v2)
	{
		return BpVec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	//====== BpVec4 ===========
	class BpVec4
	{
	public:
		BpVec4()
			:x(0.f), y(0.f), z(0.f), w(0.f) {}

		BpVec4(f32 _x, f32 _y, f32 _z, f32 _w)
			:x(_x), y(_y), z(_z), w(_w) {}

		inline BpVec4 operator*(f32 f) const;

		inline bool IsFinite() const {
			return BpIsFinite(x) && BpIsFinite(y) && BpIsFinite(z) && BpIsFinite(w);
		}

		static const BpVec4 g_InvalidBpVec4;

	public:
		f32 x, y, z, w;
	};

	inline BpVec4 BpVec4::operator*(f32 f) const
	{
		return BpVec4(x * f, y * f, z * f, w * f);
	}

	inline BpVec4 operator *(const f32 f, const BpVec4 v)
	{
		return v * f;
	}

}