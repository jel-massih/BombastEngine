#pragma once
namespace bPhysics
{
	class BpVec3
	{
	public:
		BpVec3()
			:x(0.f), y(0.f), z(0.f) {}

		BpVec3(float _x, float _y, float _z)
			:x(_x), y(_y), z(_z) {}

		inline BpVec3 operator*(float f) const;

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

}