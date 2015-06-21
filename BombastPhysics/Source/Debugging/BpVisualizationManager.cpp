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

BpDebugLine* BpVisualizationManager::AddDebugLine(BpVec3 position, BpVec3 extent, BpVec3 color = BpVec3(255, 0, 0))
{
	BpDebugLine* pLine = BP_NEW BpDebugLine(position, extent, color);
	m_renderBuffer.m_debugShapes.push_back(pLine);

	return pLine;
}

BpDebugSphere* BpVisualizationManager::AddDebugSphere(BpVec3 center, float radius, BpVec3 color)
{
	BpDebugSphere* pSphere = BP_NEW BpDebugSphere(center, radius, color);
	m_renderBuffer.m_debugShapes.push_back(pSphere);
	
	return pSphere;
}

BpDebugBox* BpVisualizationManager::AddDebugBox(BpVec3 center, BpVec3 extent, BpVec3 color = BpVec3(255, 0, 0))
{
	BpDebugBox* pBox = BP_NEW BpDebugBox(center, extent, color);
	m_renderBuffer.m_debugShapes.push_back(pBox);

	return pBox;
}