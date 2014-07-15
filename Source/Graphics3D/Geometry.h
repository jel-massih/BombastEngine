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


#endif