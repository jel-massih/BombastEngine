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
}