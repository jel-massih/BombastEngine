#pragma once

#include "../Foundation/BpMath.h"
#include <vector>
#include "../Debugging/BpVisualizationManager.h"

namespace bPhysics
{
	class BpRigidDynamic;
	class BpActor;
	class BpVisualizationManager;
	class BpDebugRenderBuffer;

	class BpSceneDesc
	{
	public:
		inline BpSceneDesc();

	public:

		BpVec3 gravity;

		//Contact with relative velocity below this will not bounce. Try 0.2 * gravity
		f32 bounceThresholdVelocity;
	};

	class BpScene
	{
	public:
		BpScene();
		~BpScene();

		void Release();

		void InitFromSceneDesc(const BpSceneDesc& sceneDesc);

		void SetGravity(const BpVec3& vec) { m_gravity = vec; }
		inline BpVec3 GetGravity() const { return m_gravity; }

		void SetBounceThresholdVelocity(const f32 f) { m_bounceThresholdVelocity = f; }
		f32 GetBounceThresholdVelocity() const { return m_bounceThresholdVelocity; }

		//Add Actor to proper store based off type
		void AddActor(BpActor* actor);
		
		void RemoveActor(BpActor* actor);

		//Simulate the scene forward by the given timestep
		void Simulate(f32 timestep);

		//Debug Visualization
		void SetVisualizationParameter(BpVisualizationParams parameter, f32 newScale);
		f32 GetVisualizationParameter(BpVisualizationParams parameter) const;

		const BpDebugRenderBuffer& GetRenderBuffer() const;

	private:
		BpVec3 m_gravity;
		f32 m_bounceThresholdVelocity;

		std::vector<BpRigidDynamic*> m_sceneRigidDynamics;

		BpVisualizationManager m_visualizationManager;
	};

	inline BpSceneDesc::BpSceneDesc()
		: gravity(BpVec3()),
		bounceThresholdVelocity(2)
	{
	}
}