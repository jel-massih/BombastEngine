#pragma once

#include "BpVisualizationParams.h"
#include "BpDebugRenderBuffer.h"
#include "../Actors/BpRigidActor.h"
#include "../Geometry/BpShape.h"

namespace bPhysics
{
	class BpVisualizationManager
	{
	public:
		void SetVisualizationParameter(BpVisualizationParams parameter, float newScale);
		float GetVisualizationParameter(BpVisualizationParams parameter) const;

		const BpDebugRenderBuffer& GetDebugRenderBuffer() const { return m_renderBuffer; }

		void VisualizeShape(const BpShape& shape, const BpRigidActor& owner);

	private:
		float m_visualizationParams[BpVisualizationParams::VALUE_COUNT];
		BpDebugRenderBuffer m_renderBuffer;
	};

}