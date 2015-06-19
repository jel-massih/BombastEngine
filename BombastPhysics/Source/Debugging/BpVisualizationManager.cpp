#include "../msvc/stdafx.h"
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

BpDebugSphere* BpVisualizationManager::AddDebugSphere(BpVec3 center, float radius, BpVec3 color)
{
	BpDebugSphere* pSphere = BP_NEW BpDebugSphere(center, radius, color);
	m_renderBuffer.m_debugSpheres.push_back(pSphere);
	
	return pSphere;
}