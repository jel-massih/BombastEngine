#include "BpShape.h"

using namespace bPhysics;

BpShape::BpShape(BpGeometry* geometry, bool isExclusive)
{
	m_geometry = geometry;
}