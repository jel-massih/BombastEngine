#pragma once

#include "../Foundation/BpMath.h"

namespace bPhysics
{
	class BpDebugShape;

	//Specifies the type of BpGeometry Object
	struct BpGeometryType
	{
		enum Type
		{
			SPHERE,
			BOX,
			CAPSULE
		};
	};

	class BpGeometry
	{
	public:
		inline BpGeometryType::Type GetType() const { return m_type; }

	protected:
		inline BpGeometry(BpGeometryType::Type type) : m_type(type) {}
		BpGeometryType::Type m_type;
	};

	class BpGeometryHolder
	{
	public:
		BpGeometryHolder() : m_pGeometry(nullptr) {}
		~BpGeometryHolder();

		inline BpGeometry& GetGeometry() const { return *m_pGeometry; }

		void SetGeometry(const BpGeometry& geometry);

	private:
		BpGeometry* m_pGeometry;
	};

	class BpRaycastData
	{
	public:
		BpRaycastData(const BpVec3& startPos, const BpVec3& dir, float endPointTime);
		const BpVec3 GetImpactPoint() const;

		BpVec3 start;
		BpVec3 dir;
		float time;

		float timeToImpact;
		BpVec3 normal;
	};

	class BpAABB
	{
	public:
		BpAABB();
		BpAABB(BpVec3 min, BpVec3 max);

		BpVec3 GetMin() const;
		BpVec3 GetMax() const;

		void SetMin(BpVec3& vec) { m_min = vec; }
		void SetMax(BpVec3& vec) { m_max = vec; }

		inline bool Contains(const BpAABB& other) const;
		inline bool Contains(const BpVec3& point) const;
		inline float GetSurfaceArea() const;
	private:
		BpVec3 m_min, m_max;
	};

	inline bool AABBtoAABB(const BpAABB& a, const BpAABB& b)
	{
		if (a.GetMax().x < b.GetMin().x || a.GetMin().x > b.GetMax().x)
		{
			return false;
		}

		if (a.GetMax().y < b.GetMin().y || a.GetMin().y > b.GetMax().y)
		{
			return false;
		}

		if (a.GetMax().z < b.GetMin().z || a.GetMin().z > b.GetMax().z)
		{
			return false;
		}

		return true;
	}

	inline bool BpAABB::Contains(const BpAABB& other) const
	{
		return (
			m_min.x <= other.m_min.x &&
			m_min.y <= other.m_min.y &&
			m_min.z <= other.m_min.z &&
			m_max.x >= other.m_max.x &&
			m_max.y >= other.m_max.y &&
			m_max.z >= other.m_max.z
			);
	}

	inline bool BpAABB::Contains(const BpVec3& pos) const
	{
		return
			m_min.x <= pos.x &&
			m_min.y <= pos.y &&
			m_min.z <= pos.z &&
			m_max.x >= pos.x &&
			m_max.y >= pos.y &&
			m_max.z >= pos.z;
	}

	inline float BpAABB::GetSurfaceArea() const
	{
		float x = m_max.x - m_min.x;
		float y = m_max.y - m_min.y;
		float z = m_max.z - m_min.z;
		
		return 2.0f * ((x * y) + (x * z) + (y * z));
	}
}