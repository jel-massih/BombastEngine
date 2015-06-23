#pragma once

#include "BpVisualizationParams.h"
#include "BpDebugRenderBuffer.h"

namespace bPhysics
{

	class BpVisualizationManager
	{
	public:
		void SetVisualizationParameter(BpVisualizationParams parameter, float newScale);
		float GetVisualizationParameter(BpVisualizationParams parameter) const;

		const BpDebugRenderBuffer& GetDebugRenderBuffer() const { return m_renderBuffer; }

		inline void AddDebugShape(BpDebugShape* shape) { m_renderBuffer.m_debugShapes.push_back(shape); }
		BpDebugLine* AddDebugLine(BpVec3 position, BpVec3 extent, BpVec3 color = BpVec3(255, 0, 0));
		BpDebugSphere* AddDebugSphere(BpVec3 center, float radius, BpVec3 color = BpVec3(255, 0, 0));
		BpDebugBox* AddDebugBox(BpVec3 center, BpVec3 extent, BpVec3 color = BpVec3(255, 0, 0));

	private:
		float m_visualizationParams[BpVisualizationParams::VALUE_COUNT];
		BpDebugRenderBuffer m_renderBuffer;
	};

}