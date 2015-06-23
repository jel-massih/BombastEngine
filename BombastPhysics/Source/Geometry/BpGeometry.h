#pragma once

namespace bPhysics
{
	class BpDebugShape;

	//Specifies the type of BpGeometry Object
	struct BpGeometryType
	{
		enum Type
		{
			SPHERE,
			BOX
		};
	};

	class BpGeometry
	{
	public:
		inline BpGeometryType::Type GetType() const { return m_type; }

		virtual BpDebugShape* CreateDebugShape() const = 0;

	protected:
		inline BpGeometry(BpGeometryType::Type type) : m_type(type) {}
		BpGeometryType::Type m_type;
	};

}