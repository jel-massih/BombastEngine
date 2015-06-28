#include "../BpPhysicsApi.h"
#include "BpGeometry.h"

using namespace bPhysics;

BpGeometryHolder::~BpGeometryHolder() 
{
	BP_SAFE_DELETE(m_pGeometry); 
}

void BpGeometryHolder::SetGeometry(const BpGeometry& geometry)
{
	BpGeometryType::Type type = geometry.GetType();

	BP_SAFE_DELETE(m_pGeometry);

	switch (type) 
	{
		case BpGeometryType::SPHERE:
		{
			BpGeometrySphere sphere = static_cast<const BpGeometrySphere&>(geometry);
			m_pGeometry = BP_NEW BpGeometrySphere(sphere);
			break;
		}
		case BpGeometryType::BOX:
		{
			BpGeometryBox box = static_cast<const BpGeometryBox&>(geometry);
			m_pGeometry = BP_NEW BpGeometryBox(box);
			break;
		}
		default:
			break;
	}
}