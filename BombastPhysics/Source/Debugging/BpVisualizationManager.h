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

		bool AddDebugSphere(BpVec3 center, float radius, BpVec3 color = BpVec3(0, 255, 0)) { return m_renderBuffer.AddDebugSphere(center, radius, color); }

	private:
		float m_visualizationParams[BpVisualizationParams::VALUE_COUNT];
		BpDebugRenderBuffer m_renderBuffer;
	};

}