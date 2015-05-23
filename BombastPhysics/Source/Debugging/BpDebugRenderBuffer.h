#pragma once

#include "../Foundation/BpMath.h"
#include <vector>

namespace bPhysics
{
	class BpDebugLine
	{
	public:
		BpVec3 point1;
		BpVec3 point2;
		BpVec3 color;
	};

	class BpDebugSphere
	{
	public:
		BpVec3 center;
		float radius;
		BpVec3 color;
	};

	class BpDebugRenderBuffer
	{
	public:
		BpDebugLine* GetLines() { return &m_debugLines.front(); }
		unsigned int GetLinesCount() { return m_debugLines.size(); }

		const BpDebugSphere* GetSpheres() const { return m_debugSpheres.size() > 0 ? &m_debugSpheres.front() : nullptr; }
		const unsigned int GetSphereCount() const { return m_debugSpheres.size(); }

	public:
		std::vector<BpDebugLine> m_debugLines;
		std::vector<BpDebugSphere> m_debugSpheres;
	};
}