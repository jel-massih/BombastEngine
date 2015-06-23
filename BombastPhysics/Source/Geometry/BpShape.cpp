#include "BpShape.h"

#include "BpGeometryBox.h"
#include "BpGeometrySphere.h"
#include "../Debugging/BpDebugRenderBuffer.h"

using namespace bPhysics;

BpShape::BpShape(BpGeometry* geometry, bool isExclusive)
{
	m_geometry = geometry;
}

BpDebugShape* BpGeometrySphere::CreateDebugShape() const
{
	BpDebugSphere* sphere = BP_NEW BpDebugSphere;
	sphere->radius = radius;
	return sphere;
}

BpDebugShape* BpGeometryBox::CreateDebugShape() const
{
	BpDebugBox* box = BP_NEW BpDebugBox;
	box->extent = BpVec3(eX, eY, eZ);
	return box;
}