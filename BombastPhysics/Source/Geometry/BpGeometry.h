#pragma once

namespace bPhysics
{

	//Specifies the type of BpGeometry Object
	struct BpGeometryType
	{
		enum Type
		{
			SPHERE
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

}