#ifndef GEOMETRY_H
#define GEOMETRY_H

using namespace DirectX;

class Vec3 : public XMFLOAT3
{
public:
	inline float Length() { float f = 0.0	;  XMStoreFloat(&f, XMVector3Length(XMLoadFloat3(this))); return f; }
	inline Vec3 Normalize() { Vec3 dest;  XMStoreFloat3(&dest, XMVector3Normalize(XMLoadFloat3(this))); return dest; }
	inline float Dot(const Vec3 &b) { float f = 0.0; XMStoreFloat(&f, XMVector3Dot(XMLoadFloat3(this), XMLoadFloat3(&b))); return f; }
	inline Vec3 Cross(const Vec3 &b) const { Vec3 out; XMStoreFloat3(&out, XMVector3Cross(XMLoadFloat3(this), XMLoadFloat3(&b))); return out;}

	Vec3(XMFLOAT3 &v3) { x = v3.x; y = v3.y; z = v3.z; }
	Vec3() : XMFLOAT3() { x = 0; y = 0; z = 0; }
	Vec3(const float _x, const float _y, const float _z) { x = _x; y = _y; z = _z; }
	Vec3(const double _x, const double _y, const double _z) { x = (float)_x; y = (float)_y; z = (float)_z; }
};

extern Vec3 g_Up;
extern Vec3 g_Right;
extern Vec3 g_Forward;

class Mat4x4 : public XMFLOAT4X4
{
public:
	inline void SetPosition(Vec3 const &pos);
	inline void SetScale(Vec3 const &scale);

	inline Vec3 GetPosition() const;
	inline Vec3 GetDirection() const;
	inline Vec3 GetUp() const;
	inline Vec3 GetRight() const;
	inline Vec3 GetYawPitchRoll() const;
	inline Vec3 GetScale() const;
	inline Vec3 Xform(Vec3 &v) const;
	inline Mat4x4 Inverse() const;

	Mat4x4(XMFLOAT4X4& mat) { memcpy(&m, &mat.m, sizeof(mat.m)); }
	Mat4x4() : XMFLOAT4X4(){}

	static const Mat4x4 g_Identity;

	inline void BuildTranslation(const Vec3 &pos);
	inline void BuildTranslation(const float x, const float y, const float z);
	inline void BuildRotationX(const float radians) { XMStoreFloat4x4(this, XMMatrixRotationX(radians)); }
	inline void BuildRotationY(const float radians) { XMStoreFloat4x4(this, XMMatrixRotationY(radians)); }
	inline void BuildRotationZ(const float radians) { XMStoreFloat4x4(this, XMMatrixRotationZ(radians)); }
	inline void BuildScale(const float x, const float y, const float z);
};

inline void Mat4x4::SetPosition(Vec3 const &pos)
{
	m[3][0] = pos.x;
	m[3][1] = pos.y;
	m[3][2] = pos.z;
	m[3][3] = 1.0f;
}

inline void Mat4x4::SetScale(Vec3 const &scale)
{
	m[1][1] = scale.x;
	m[2][2] = scale.y;
	m[3][3] = scale.z;
}

inline Vec3 Mat4x4::GetPosition() const
{
	return Vec3(m[3][0], m[3][1], m[3][2]);
}

inline Vec3 Mat4x4::GetDirection() const
{
	Mat4x4 rot = *this;
	rot.SetPosition(Vec3(0.f, 0.f, 0.f));
	Vec3 forward = rot.Xform(g_Forward);
	return forward;
}

inline Vec3 Mat4x4::GetRight() const
{
	Mat4x4 rot = *this;
	rot.SetPosition(Vec3(0.f, 0.f, 0.f));
	Vec3 right = rot.Xform(g_Right);
	return right;
}

inline Vec3 Mat4x4::GetUp() const
{
	Mat4x4 rot = *this;
	rot.SetPosition(Vec3(0.f, 0.f, 0.f));
	Vec3 up = rot.Xform(g_Up);
	return up;
}

inline Vec3 Mat4x4::GetYawPitchRoll() const
{
	float yaw, pitch, roll;

	pitch = asin(-_32);

	double threshold = 0.001; //UGLY HARDCODE MAGIC NUMBER
	double test = cos(pitch);

	if (test > threshold)
	{
		roll = atan2(_12, _22);
		yaw = atan2(_31, _33);
	}
	else
	{
		roll = atan2(-_21, _11);
		yaw = 0.0;
	}

	return Vec3(yaw, pitch, roll);
}

inline Vec3 Mat4x4::GetScale() const
{
	return Vec3(m[0][0], m[1][1], m[2][2]);
}

inline Vec3 Mat4x4::Xform(Vec3& v) const
{
	Vec3 temp(v);
	Vec3 out;
	XMStoreFloat3(&out, XMVector4Transform(XMLoadFloat3(&temp), XMLoadFloat4x4(this)));
	return Vec3(out.x, out.y, out.z);
}

inline Mat4x4 Mat4x4::Inverse() const
{
	Mat4x4 out;
	XMStoreFloat4x4(&out, XMMatrixInverse(NULL, XMLoadFloat4x4(this)));
	return out;
}

inline void Mat4x4::BuildTranslation(const Vec3& pos)
{
	*this = Mat4x4::g_Identity;
	m[3][0] = pos.x;
	m[3][1] = pos.y;
	m[3][2] = pos.z;
}

inline void Mat4x4::BuildTranslation(const float x, const float y, const float z)
{
	*this = Mat4x4::g_Identity;
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
}

inline void Mat4x4::BuildScale(const float x, const float y, const float z)
{
	*this = Mat4x4::g_Identity;
	m[1][1] = x;
	m[2][2] = y;
	m[3][3] = z;
}

inline Mat4x4 operator * (const Mat4x4& a, const Mat4x4& b)
{
	Mat4x4 out;
	XMStoreFloat4x4(&out, XMMatrixMultiply(XMLoadFloat4x4(&a), XMLoadFloat4x4(&b)));
	return out;
}

#endif