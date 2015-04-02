#pragma once

#include "BpVisualizationParams.h"

namespace bPhysics
{

	class BpVisualizationManager
	{
	public:
		void SetVisualizationParameter(BpVisualizationParams parameter, float newScale);
		float GetVisualizationParameter(BpVisualizationParams parameter);

	private:
		float m_visualizationParams[BpVisualizationParams::VALUE_COUNT];
	};

}