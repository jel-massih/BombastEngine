#pragma once

#include "BpVisualizationParams.h"
#include "BpDebugRenderBuffer.h"

namespace bPhysics
{

	class BpVisualizationManager
	{
	public:
		void SetVisualizationParameter(BpVisualizationParams parameter, float newScale);
		float GetVisualizationParameter(BpVisualizationParams parameter);

		BpDebugRenderBuffer& GetDebugRenderBuffer() { return m_renderBuffer; }

	private:
		float m_visualizationParams[BpVisualizationParams::VALUE_COUNT];
		BpDebugRenderBuffer m_renderBuffer;
	};

}