#include "BpDebugRenderBuffer.h"

namespace bPhysics {
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