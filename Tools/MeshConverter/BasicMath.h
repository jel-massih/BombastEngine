#pragma once

template <typename T>
struct Vector3
{
	T x;
	T y;
	T z;

	Vector3(T _x = 0, T _y = 0, T _z = 0)
		: x(_x), y(_y), z(_z)
	{}
};

template <typename T>
struct Vector2
{
	T x;
	T y;

	Vector2(T _x = 0, T _y = 0)
		: x(_x), y(_y)
	{}
};

template<typename T>
const Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

template<typename T>
const Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template<typename T>
const Vector3<T> operator/(const Vector3<T>& lhs, const float& rhs)
{
	return Vector3<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

template<typename T>
const Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b)
{
	return Vector3<T>((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
}