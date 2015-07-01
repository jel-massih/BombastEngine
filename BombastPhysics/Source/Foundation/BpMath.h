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
		BpMat4x4(){}
		BpMat4x4(float _00, float _01, float _02, float _03,
			float _10, float _11, float _12, float _13,
			float _20, float _21, float _22, float _23,
			float _30, float _31, float _32, float _33) : col0(_00, _01, _02, _03), col1(_10, _11, _12, _13), col2(_20, _21, _22, _23), col3(_30, _31, _32, _33) {}
		BpMat4x4(BpVec4 col0, BpVec4 col1, BpVec4 col2, BpVec4 cole3) : col0(col0), col1(col1), col2(col2), col3(col3) {}

		inline bool IsValid() const
		{
			return col0.IsFinite() && col1.IsFinite() && col2.IsFinite() && col3.IsFinite();
		}

		inline BpVec3 GetPosition() const;
		inline void SetPosition(const BpVec3& pos);

		inline BpVec3 GetScale() const;
		inline void SetScale(const BpVec3& scale);
		inline void SetScale(const float x, const float y, const float z);

		inline void BuildScale(const float x, const float y, const float z);
		inline void BuildScale(const BpVec3& scale);

		static const BpMat4x4 g_InvalidBpMat4x4;
		static const BpMat4x4 g_Identity;

		BpVec4 col0, col1, col2, col3;
	};

	inline BpVec3 BpMat4x4::GetPosition() const
	{
		return BpVec3(col3.x, col3.y, col3.z);
	}

	inline BpMat4x4 operator * (const BpMat4x4& a, const BpMat4x4& b)
	{
		BpMat4x4 out;

		out.col0.x = (a.col0.x * b.col0.x) + (a.col1.x * b.col0.y) + (a.col2.x * b.col0.z) + (a.col3.x * b.col0.w);
		out.col0.y = (a.col0.x * b.col1.x) + (a.col1.x * b.col1.y) + (a.col2.x * b.col1.z) + (a.col3.x * b.col1.w);
		out.col0.z = (a.col0.x * b.col2.x) + (a.col1.x * b.col2.y) + (a.col2.x * b.col2.z) + (a.col3.x * b.col2.w);
		out.col0.w = (a.col0.x * b.col3.x) + (a.col1.x * b.col3.y) + (a.col2.x * b.col3.z) + (a.col3.x * b.col3.w);

		out.col1.x = (a.col0.y * b.col0.x) + (a.col1.y * b.col0.y) + (a.col2.y * b.col0.z) + (a.col3.y * b.col0.w);
		out.col1.y = (a.col0.y * b.col1.x) + (a.col1.y * b.col1.y) + (a.col2.y * b.col1.z) + (a.col3.y * b.col1.w);
		out.col1.z = (a.col0.y * b.col2.x) + (a.col1.y * b.col2.y) + (a.col2.y * b.col2.z) + (a.col3.y * b.col2.w);
		out.col1.w = (a.col0.y * b.col3.x) + (a.col1.y * b.col3.y) + (a.col2.y * b.col3.z) + (a.col3.y * b.col3.w);

		out.col2.x = (a.col0.z * b.col0.x) + (a.col1.z * b.col0.y) + (a.col2.z * b.col0.z) + (a.col3.z * b.col0.w);
		out.col2.y = (a.col0.z * b.col1.x) + (a.col1.z * b.col1.y) + (a.col2.z * b.col1.z) + (a.col3.z * b.col1.w);
		out.col2.z = (a.col0.z * b.col2.x) + (a.col1.z * b.col2.y) + (a.col2.z * b.col2.z) + (a.col3.z * b.col2.w);
		out.col2.w = (a.col0.z * b.col3.x) + (a.col1.z * b.col3.y) + (a.col2.z * b.col3.z) + (a.col3.z * b.col3.w);

		out.col3.x = (a.col0.w * b.col0.x) + (a.col1.w * b.col0.y) + (a.col2.w * b.col0.z) + (a.col3.w * b.col0.w);
		out.col3.y = (a.col0.w * b.col1.x) + (a.col1.w * b.col1.y) + (a.col2.w * b.col1.z) + (a.col3.w * b.col1.w);
		out.col3.z = (a.col0.w * b.col2.x) + (a.col1.w * b.col2.y) + (a.col2.w * b.col2.z) + (a.col3.w * b.col2.w);
		out.col3.w = (a.col0.w * b.col3.x) + (a.col1.w * b.col3.y) + (a.col2.w * b.col3.z) + (a.col3.w * b.col3.w);

		return out;
	}

	inline void BpMat4x4::SetPosition(const BpVec3& pos)
	{
		col3.x = pos.x;
		col3.y = pos.y;
		col3.z = pos.z;
	}

	inline void BpMat4x4::BuildScale(const float x, const float y, const float z)
	{
		*this = BpMat4x4::g_Identity;
		SetScale(x, y, z);
	}

	inline void BpMat4x4::BuildScale(const BpVec3& scale)
	{
		*this = BpMat4x4::g_Identity;
		SetScale(scale);
	}

	inline void BpMat4x4::SetScale(const BpVec3& scale)
	{
		col0.x = scale.x;
		col1.y = scale.y;
		col2.z = scale.z;
	}

	inline void BpMat4x4::SetScale(const float x, const float y, const float z)
	{
		col0.x = x;
		col1.y = y;
		col2.z = z;
	}
	
	inline BpVec3 BpMat4x4::GetScale() const
	{
		return BpVec3(col0.x, col1.y, col2.z);
	}
}