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

		BpDebugSphere* GetSpheres() { return &m_debugSpheres.front(); }
		unsigned int GetSphereCount() { return m_debugSpheres.size(); }

	public:
		std::vector<BpDebugLine> m_debugLines;
		std::vector<BpDebugSphere> m_debugSpheres;
	};
}