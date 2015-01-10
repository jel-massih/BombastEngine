#include "../BombastEngineStd.h"
#include "Geometry.h"

const Mat4x4 Mat4x4::g_Identity(XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

const Vec3 Vec3::g_InvalidVec3(-DBL_MAX, -DBL_MAX, -DBL_MAX);
const Vec3 Vec3::g_IdentityVec3(1, 1, 1);

Vec3 Vec3::operator*(float f) const
{
	Vec3 v = Vec3(x*f, y*f, z*f);
	return v;
}

Vec3& Vec3::operator*=(const Vec3& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;

	return *this;
}

Vec3& Vec3::operator*=(const float& rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;

	return *this;
}

Vec3 Vec3::operator-(Vec3 v) const
{
	Vec3 v2 = Vec3(x - v.x, y - v.y, z - v.z);

	return v2;
}

Vec3 Vec3::operator+(Vec3 v) const
{
	Vec3 v2 = Vec3(x + v.x, y + v.y, z + v.z);

	return v2;
}

Vec3& Vec3::operator+=(const Vec3 v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

bool Vec3::operator==(const Vec3 v)
{
	return (v.x == x && v.y == y && v.z == z);
}

Vec3 operator* (float f, const Vec3& vec)
{
	return vec * f;
}

Vec4 Vec4::operator+(Vec4& rhs) const
{
	Vec4 v2 = Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);

	return v2;
}

Vec4& Vec4::operator*=(const float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;

	return *this;
}

bool Mat4x4::operator!=(const Mat4x4 rhs)
{
	return !(
		_11 == rhs._11 &&
		_12 == rhs._12 &&
		_13 == rhs._13 &&
		_14 == rhs._14 &&
		_21 == rhs._21 &&
		_22 == rhs._22 &&
		_23 == rhs._23 &&
		_24 == rhs._24 &&
		_31 == rhs._31 &&
		_32 == rhs._32 &&
		_33 == rhs._33 &&
		_34 == rhs._34 &&
		_41 == rhs._41 &&
		_42 == rhs._42 &&
		_43 == rhs._43 &&
		_44 == rhs._44
		);
}

bool Plane::Inside(const Vec3& point) const
{
	XMFLOAT4 planeDotProduct;
	
	XMStoreFloat4(&planeDotProduct, XMPlaneDotCoord(XMLoadFloat4(this), point.Vect()));
	
	return (planeDotProduct.x >= 0.0f);
}

bool Plane::Inside(const Vec3& point, const float radius) const
{
	XMFLOAT4 planeDotProduct;
	XMStoreFloat4(&planeDotProduct, XMPlaneDotCoord(XMLoadFloat4(this), point.Vect()));

	return (planeDotProduct.x >= -radius);
}

Frustum::Frustum()
{
	m_fov = BE_PI / 4.0f; //Default to 90
	m_aspect = 1.0f;
	m_near = 1.0f;
	m_far = 1000.0f;
}

bool Frustum::Inside(const Vec3& point) const
{
	for (int i = 0; i < NumPlanes; i++)
	{
		if (!m_planes[i].Inside(point))
		{
			return false;
		}
	}

	return true;
}

bool Frustum::Inside(const Vec3& point, const float radius) const
{
	for (int i = 0; i < NumPlanes; i++)
	{
		if (!m_planes[i].Inside(point, radius))
		{
			return false;
		}
	}

	return true;
}

void Frustum::Init(const float fov, const float aspect, const float nearClip, const float farClip)
{
	m_fov = fov;
	m_aspect = aspect;
	m_near = nearClip;
	m_far = farClip;
	
	float tanFovOver2 = (float)tan(m_fov / 2.0f);
	Vec3 nearRight = (m_near * tanFovOver2) * m_aspect * g_Right;
	Vec3 farRight = (m_far * tanFovOver2) * m_aspect * g_Right;
	Vec3 nearUp = (m_near * tanFovOver2) * g_Up;
	Vec3 farUp = (m_far * tanFovOver2) * g_Up;

	m_nearClip[0] = (m_near * g_Forward) - nearRight + nearUp;
	m_nearClip[1] = (m_near * g_Forward) + nearRight + nearUp;
	m_nearClip[2] = (m_near * g_Forward) + nearRight - nearUp;
	m_nearClip[3] = (m_near * g_Forward) - nearRight - nearUp;

	m_farClip[0] = (m_far * g_Forward) - farRight + farUp;
	m_farClip[1] = (m_far * g_Forward) + farRight + farUp;
	m_farClip[2] = (m_far * g_Forward) + farRight - farUp;
	m_farClip[3] = (m_far * g_Forward) - farRight - farUp;

	//Construct 6 planes from the 8 points
	//Normals point awar if use CCW verts

	Vec3 origin(0.0f, 0.0f, 0.0f);
	m_planes[Near].Init(m_nearClip[2], m_nearClip[1], m_nearClip[0]);
	m_planes[Far].Init(m_farClip[0], m_farClip[1], m_farClip[2]);
	m_planes[Right].Init(m_farClip[2], m_farClip[1], origin);
	m_planes[Top].Init(m_farClip[1], m_farClip[0], origin);
	m_planes[Left].Init(m_farClip[0], m_farClip[3], origin);
	m_planes[Bottom].Init(m_farClip[3], m_farClip[2], origin);
}

void Frustum::Render()
{
	//@TODO: not Implemented Yet
}