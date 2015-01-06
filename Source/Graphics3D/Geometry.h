#pragma once

using namespace DirectX;

const float  BE_PI = 3.14159265358979f;
const float  BE_2PI = 2 * BE_PI;

class Vec4 : public XMFLOAT4
{
public:
	inline float Length() { XMVectorGetX(XMVector4Length(XMLoadFloat4(this))); }
	inline Vec4* Normalize()
	{
		Vec4* v;
		XMStoreFloat4(v, XMVector4Normalize(XMLoadFloat4(this)));

		return v;
	}
	inline float Dot(const Vec4 &b)
	{
		return XMVectorGetX(XMVector4Dot(XMLoadFloat4(this), XMLoadFloat4(&b)));
	}
	inline float Dot(const XMVECTOR &v)
	{
		return XMVectorGetX(XMVector4Dot(XMLoadFloat4(this), v));
	}

	Vec4(XMFLOAT4& v4) { x = v4.x; y = v4.y; z = v4.z; w = v4.w; }
	Vec4() : XMFLOAT4() {}
	Vec4(const float _x, const float _y, const float _z, const float _w) { x = _x; y = _y; z = _z; w = _w; }
	//Vec4(const Vec3& v3) { x = v3.x; y = v3.y; z = v3.z; w = 1.0f; }

	Vec4 operator+(Vec4& rhs) const;
	Vec4& operator*=(const float rhs);
};

extern Vec4 g_Up4;
extern Vec4 g_Right4;
extern Vec4 g_Forward4;

class Vec3 : public XMFLOAT3
{
public:
	inline float Length() { float f = 0.0	;  XMStoreFloat(&f, XMVector3Length(XMLoadFloat3(this))); return f; }
	inline Vec3 Normalize() { Vec3 dest;  XMStoreFloat3(&dest, XMVector3Normalize(XMLoadFloat3(this))); return dest; }
	inline float Dot(const Vec3 &b) { float f = 0.0; XMStoreFloat(&f, XMVector3Dot(XMLoadFloat3(this), XMLoadFloat3(&b))); return f; }
	inline Vec3 Cross(const Vec3 &b) const { Vec3 out; XMStoreFloat3(&out, XMVector3Cross(XMLoadFloat3(this), XMLoadFloat3(&b))); return out;}
	inline const XMVECTOR Vect() const { return XMLoadFloat3(this); }

	Vec3(XMFLOAT3 &v3) { x = v3.x; y = v3.y; z = v3.z; }
	Vec3() : XMFLOAT3() { x = 0; y = 0; z = 0; }
	Vec3(const float _x, const float _y, const float _z) { x = _x; y = _y; z = _z; }
	Vec3(const double _x, const double _y, const double _z) { x = (float)_x; y = (float)_y; z = (float)_z; }
	Vec3(const int _x, const int _y, const int _z) { x = (float)_x; y = (float)_y; z = (float)_z; }
	inline Vec3(const class Vec4& v4);

	static const Vec3 g_InvalidVec3;

	Vec3 operator*(float f) const;
	Vec3& operator*=(const Vec3& rhs);
	Vec3& operator*=(const float& rhs);
	Vec3 operator-(Vec3 v) const;
	Vec3 operator+(Vec3 v) const;
	Vec3& operator+=(const Vec3 v);
	bool operator==(const Vec3 v);
};

inline Vec3::Vec3(const Vec4& v4) { x = v4.x; y = v4.y; z = v4.z; }

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
	inline Vec4 Xform(Vec4 &v) const;
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
	inline void BuildYawPitchRoll(const float yawRadians, const float pitchRadians, const float rollRadians)
	{
		XMStoreFloat4x4(this, XMMatrixRotationRollPitchYaw(yawRadians, pitchRadians, rollRadians));
	}
	inline void BuildYawPitchRoll(const Vec3 rotationRadians)
	{
		XMStoreFloat4x4(this, XMMatrixRotationRollPitchYaw(rotationRadians.x, rotationRadians.y, rotationRadians.z));
	}

	bool operator!=(const Mat4x4 rhs);
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

inline Vec4 Mat4x4::Xform(Vec4& v) const
{
	Vec4 temp;
	XMStoreFloat4(&temp, XMVector4Transform(XMLoadFloat4(&v), XMLoadFloat4x4(this)));
	return temp;
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

//Plane
class Plane : public XMFLOAT4
{
public:
	inline void Normalize();

	inline void Init(const Vec3& p0, const Vec3& p1, const Vec3& p2);
	bool Inside(const Vec3& point, const float radius) const;
	bool Inside(const Vec3& point) const;
};

inline void Plane::Normalize()
{
	float length;
	length = sqrt(x*x + y*y + z*z);
	x = x / length;
	y = y / length;
	z = z / length;
	w = w / length;
}

inline void Plane::Init(const Vec3& p0, const Vec3& p1, const Vec3& p2)
{
	XMStoreFloat4(this, XMPlaneFromPoints(p0.Vect(), p1.Vect(), p2.Vect()));
	Normalize();
}

//Frustum
class Frustum
{
public:
	enum Side{Near, Far, Top, Right, Bottom, Left, NumPlanes};

	Plane m_planes[NumPlanes];	//Planes of Frustum in Camera Space
	Vec3 m_nearClip[4];		    //Verts of near clip plane in cam space
	Vec3 m_farClip[4];			//verts of far clip plane

	float m_fov;
	float m_aspect;
	float m_near;
	float m_far;

public:
	Frustum();

	bool Inside(const Vec3& point) const;
	bool Inside(const Vec3& point, const float radius) const;
	const Plane& Get(Side side) { return m_planes[side]; }
	void SetFov(float fov) { m_fov = fov; Init(m_fov, m_aspect, m_near, m_far); }
	void SetAspect(float aspect) { m_aspect = aspect; Init(m_fov, m_aspect, m_near, m_far); }
	void SetNear(float nearClip) { m_near = nearClip; Init(m_fov, m_aspect, m_near, m_far); }
	void SetFar(float farClip) { m_far = farClip; Init(m_fov, m_aspect, m_near, m_far); }
	void Init(const float fov, const float aspect, const float nearClip, const float farClip);

	void Render();
};