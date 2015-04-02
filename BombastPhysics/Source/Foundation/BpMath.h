#pragma once
#include <cmath>

namespace bPhysics
{
	//====== Misc Helpers ===========
	inline bool BpIsFinite(float f) { return isfinite(f); }

	//====== BpVec2 ===========
	class BpVec2
	{
	public:
		BpVec2()
			:x(0.f), y(0.f) {}

		BpVec2(float f)
			:x(f), y(f) {}

		BpVec2(float _x, float _y)
			:x(_x), y(_y) {}

		inline BpVec2 operator*(float f) const;

		inline BpVec2 operator+(BpVec2 v) const;
		inline BpVec2& operator+=(const BpVec2& v);

		inline bool IsFinite() const {
			return BpIsFinite(x) && BpIsFinite(y);
		}

		inline float Length() { return sqrt(x*x + y*y); }

		static const BpVec2 g_InvalidBpVec2;

	public:
		float x, y;
	};

	inline BpVec2 BpVec2::operator*(float f) const
	{
		return BpVec2(x * f, y * f);
	}

	inline BpVec2 operator *(const float f, const BpVec2 v)
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

		BpVec3(float f)
			:x(f), y(f), z(f) {}

		BpVec3(float _x, float _y, float _z)
			:x(_x), y(_y), z(_z) {}

		inline BpVec3 operator*(float f) const;

		inline BpVec3 operator+(BpVec3 v) const;
		inline BpVec3& operator+=(const BpVec3& v);

		inline bool IsFinite() const {
			return BpIsFinite(x) && BpIsFinite(y) && BpIsFinite(z);
		}

		inline float Length() { return sqrt(x*x + y*y + z*z); }

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

		inline BpVec3 GetPosition() const;
		inline void SetPosition(const BpVec3& pos);

		BpVec4 col0, col1, col2, col3;
	};

	inline BpVec3 BpMat4x4::GetPosition() const
	{
		return BpVec3(col3.x, col3.y, col3.z);
	}

	inline void BpMat4x4::SetPosition(const BpVec3& pos)
	{
		col3.x = pos.x;
		col3.y = pos.y;
		col3.z = pos.z;
	}
}