#include "BpVisualizationManager.h"
#include "../Foundation/BpAssert.h"

using namespace bPhysics;

void BpVisualizationManager::SetVisualizationParameter(BpVisualizationParams parameter, float newScale)
{
	BP_ASSERTf(parameter != BpVisualizationParams::VALUE_COUNT, "Attempting to set Invalid Visualualization Parameter");

	m_visualizationParams[parameter] = newScale;
}

float BpVisualizationManager::GetVisualizationParameter(BpVisualizationParams parameter) const
{
	BP_ASSERTf(parameter != BpVisualizationParams::VALUE_COUNT, "Attempting to Get Invalid Visualualization Parameter");

	return m_visualizationParams[parameter];
}

void BpVisualizationManager::VisualizeShape(const BpShape& shape, const BpRigidActor& owner)
{
	shape.DebugVisualize(m_renderBuffer, owner);
}