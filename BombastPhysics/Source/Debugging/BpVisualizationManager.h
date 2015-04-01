#pragma once

#include "BpVisualizationParams.h"

class BpVisualizationManager
{
public:
	void SetVisualizationParameter(BpVisualizationParams parameter, float newScale);
	float GetVisualizationParameter(BpVisualizationParams);

private:
	float m_visualizationParams[BpVisualizationParams::VALUE_COUNT];
};