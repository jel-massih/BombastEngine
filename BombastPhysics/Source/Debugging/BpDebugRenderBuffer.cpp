#include "BpDebugRenderBuffer.h"
#include "../Actors/BpRigidActor.h"
#include "../Geometry/BpShape.h"


namespace bPhysics {
	BpMat4x4 BpDebugShape::GetTransform() const
	{
		return m_pOwner->GetWorldTransform();// *m_pShape->GetLocalPose();
	}

	BpDebugRenderBuffer& operator<<(BpDebugRenderBuffer& out, const BpDebugSphere& sphere)
	{
		out.m_debugShapes.push_back(BP_NEW BpDebugSphere(sphere));

		return out;
	}

	BpDebugRenderBuffer& operator<<(BpDebugRenderBuffer& out, const BpDebugBox& box)
	{
		out.m_debugShapes.push_back(BP_NEW BpDebugBox(box));

		return out;
	}

	BpDebugRenderBuffer& operator<<(BpDebugRenderBuffer& out, const BpDebugCapsule& capsule)
	{
		out.m_debugShapes.push_back(BP_NEW BpDebugCapsule(capsule));

		return out;
	}
}