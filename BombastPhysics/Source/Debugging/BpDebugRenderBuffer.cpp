#include "BpDebugRenderBuffer.h"

using namespace bPhysics;

bool BpDebugRenderBuffer::AddDebugSphere(BpVec3 center, float radius, BpVec3 color)
{
	m_debugSpheres.push_back(BpDebugSphere(center, radius, color));
	
	return true;
}