#pragma once

#include "BpSimpleTypes.h"
#include "BpVec.h"

namespace bPhysics
{
	//====== BpMat4x4 ===========
	class BpMat4x4
	{
	public:
		BpMat4x4(){}

		BpMat4x4(f32 _00, f32 _01, f32 _02, f32 _03,
			f32 _10, f32 _11, f32 _12, f32 _13,
			f32 _20, f32 _21, f32 _22, f32 _23,
			f32 _30, f32 _31, f32 _32, f32 _33) : col0(_00, _01, _02, _03), col1(_10, _11, _12, _13), col2(_20, _21, _22, _23), col3(_30, _31, _32, _33) {}

		BpMat4x4(BpVec4 col0, BpVec4 col1, BpVec4 col2, BpVec4 cole3) : col0(col0), col1(col1), col2(col2), col3(col3) {}

		BpMat4x4(BpIDENTITY identity) : col0(1.0f, 0.0f, 0.0f, 0.0f), col1(0.0f, 1.0f, 0.0f, 0.0f), col2(0.0f, 0.0f, 1.0f, 0.0f), col3(0.0f, 0.0f, 0.0f, 1.0f) {}

		inline bool IsValid() const
		{
			return col0.IsFinite() && col1.IsFinite() && col2.IsFinite() && col3.IsFinite();
		}

		inline BpVec3 GetPosition() const;
		inline void SetPosition(const BpVec3& pos);

		inline BpVec3 GetYawPitchRoll() const;

		inline BpVec3 GetScale() const;
		inline void SetScale(const BpVec3& scale);
		inline void SetScale(const f32 x, const f32 y, const f32 z);

		inline void BuildScale(const f32 x, const f32 y, const f32 z);
		inline void BuildScale(const BpVec3& scale);

		inline void BuildYawPitchRoll(const f32 radiansX, const f32 radiansY, const f32 radiansZ);
		inline void BuildYawPitchRoll(const BpVec3& rotationRadians);
		inline void BuildTranslation(const BpVec3& pos);

		BpVec4 col0, col1, col2, col3;
	};

	inline BpMat4x4 operator * (const BpMat4x4& a, const BpMat4x4& b)
	{
		BpMat4x4 out;

		out.col0.x = (a.col0.x * b.col0.x) + (a.col1.x * b.col0.y) + (a.col2.x * b.col0.z) + (a.col3.x * b.col0.w);
		out.col0.y = (a.col0.y * b.col0.x) + (a.col1.y * b.col0.y) + (a.col2.y * b.col0.z) + (a.col3.y * b.col0.w);
		out.col0.z = (a.col0.z * b.col0.x) + (a.col1.z * b.col0.y) + (a.col2.z * b.col0.z) + (a.col3.z * b.col0.w);
		out.col0.w = (a.col0.w * b.col0.x) + (a.col1.w * b.col0.y) + (a.col2.w * b.col0.z) + (a.col3.w * b.col0.w);

		out.col1.x = (a.col0.x * b.col1.x) + (a.col1.x * b.col1.y) + (a.col2.x * b.col1.z) + (a.col3.x * b.col1.w);
		out.col1.y = (a.col0.y * b.col1.x) + (a.col1.y * b.col1.y) + (a.col2.y * b.col1.z) + (a.col3.y * b.col1.w);
		out.col1.z = (a.col0.z * b.col1.x) + (a.col1.z * b.col1.y) + (a.col2.z * b.col1.z) + (a.col3.z * b.col1.w);
		out.col1.w = (a.col0.w * b.col1.x) + (a.col1.w * b.col1.y) + (a.col2.w * b.col1.z) + (a.col3.w * b.col1.w);

		out.col2.x = (a.col0.x * b.col2.x) + (a.col1.x * b.col2.y) + (a.col2.x * b.col2.z) + (a.col3.x * b.col2.w);
		out.col2.y = (a.col0.y * b.col2.x) + (a.col1.y * b.col2.y) + (a.col2.y * b.col2.z) + (a.col3.y * b.col2.w);
		out.col2.z = (a.col0.z * b.col2.x) + (a.col1.z * b.col2.y) + (a.col2.z * b.col2.z) + (a.col3.z * b.col2.w);
		out.col2.w = (a.col0.w * b.col2.x) + (a.col1.w * b.col2.y) + (a.col2.w * b.col2.z) + (a.col3.w * b.col2.w);

		out.col3.x = (a.col0.x * b.col3.x) + (a.col1.x * b.col3.y) + (a.col2.x * b.col3.z) + (a.col3.x * b.col3.w);
		out.col3.y = (a.col0.y * b.col3.x) + (a.col1.y * b.col3.y) + (a.col2.y * b.col3.z) + (a.col3.y * b.col3.w);
		out.col3.z = (a.col0.z * b.col3.x) + (a.col1.z * b.col3.y) + (a.col2.z * b.col3.z) + (a.col3.z * b.col3.w);
		out.col3.w = (a.col0.w * b.col3.x) + (a.col1.w * b.col3.y) + (a.col2.w * b.col3.z) + (a.col3.w * b.col3.w);

		return out;
	}

	inline BpVec3 BpMat4x4::GetPosition() const
	{
		return BpVec3(col0.w, col1.w, col2.w);
	}

	inline void BpMat4x4::SetPosition(const BpVec3& pos)
	{
		col0.w = pos.x;
		col1.w = pos.y;
		col2.w = pos.z;
	}

	inline void BpMat4x4::BuildTranslation(const BpVec3& pos)
	{
		*this = BpMat4x4(BpIdentity);
		SetPosition(pos);
	}

	inline void BpMat4x4::BuildScale(const f32 x, const f32 y, const f32 z)
	{
		*this = BpMat4x4(BpIdentity);
		SetScale(x, y, z);
	}

	inline void BpMat4x4::BuildScale(const BpVec3& scale)
	{
		*this = BpMat4x4(BpIdentity);
		SetScale(scale);
	}

	inline void BpMat4x4::SetScale(const BpVec3& scale)
	{
		col0.x = scale.x;
		col1.y = scale.y;
		col2.z = scale.z;
	}

	inline void BpMat4x4::SetScale(const f32 x, const f32 y, const f32 z)
	{
		col0.x = x;
		col1.y = y;
		col2.z = z;
	}

	inline BpVec3 BpMat4x4::GetScale() const
	{
		return BpVec3(col0.x, col1.y, col2.z);
	}

	inline BpVec3 BpMat4x4::GetYawPitchRoll() const
	{
		f32 yaw, pitch, roll;

		pitch = asin(-col1.z);

		f32 threshold = (f32)0.001; //UGLY HARDCODE MAGIC NUMBER
		f64 test = cos(pitch);

		if (test > threshold)
		{
			roll = atan2(col1.x, col1.y);
			yaw = atan2(col0.z, col2.z);
		}
		else
		{
			roll = atan2(-col0.y, col0.x);
			yaw = 0.0;
		}

		return BpVec3(yaw, pitch, roll);
	}

	inline void BpMat4x4::BuildYawPitchRoll(const f32 radiansX, const f32 radiansY, const f32 radiansZ)
	{
		*this = BpMat4x4(BpIdentity);

		f32 cx = cos(radiansX);
		f32 cy = cos(radiansY);
		f32 cz = cos(radiansZ);

		f32 sx = sin(radiansX);
		f32 sy = sin(radiansY);
		f32 sz = sin(radiansZ);

		col0.x = (cz * cx) + (sz * sy * sx);
		col0.y = (-1 * cz * sx) + (sz * sy * cx);
		col0.z = sz * cy;

		col1.x = sx * cy;
		col1.y = cx * cy;
		col1.z = -1 * sy;

		col2.x = (-1 * sz * cx) + (cz * sy * sx);
		col2.y = (sx * sz) + (cz * sy * cx);
		col2.z = cz * cy;
	}

	inline void BpMat4x4::BuildYawPitchRoll(const BpVec3& rotationRadians)
	{
		BuildYawPitchRoll(rotationRadians.x, rotationRadians.y, rotationRadians.z);
	}
}