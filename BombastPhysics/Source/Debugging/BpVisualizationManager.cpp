#include "BpVisualizationManager.h"
#include "../Foundation/BpAssert.h"

void BpVisualizationManager::SetVisualizationParameter(BpVisualizationParams parameter, float newScale)
{
	BP_ASSERT(parameter != BpVisualizationParams::VALUE_COUNT, "Attempting to set Invalid Visualualization Parameter");

	m_visualizationParams[parameter] = newScale;
}

float BpVisualizationManager::GetVisualizationParameter(BpVisualizationParams parameter)
{
	BP_ASSERT(parameter != BpVisualizationParams::VALUE_COUNT, "Attempting to Get Invalid Visualualization Parameter");

	return m_visualizationParams[parameter];
}