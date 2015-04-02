#pragma once

#include "../Foundation/BpMath.h"
#include <vector>
#include "../Debugging/BpVisualizationParams.h"

class BpVisualizationManager;

namespace bPhysics
{
	class BpRigidDynamic;
	class BpActor;

	class BpSceneDesc
	{
	public:
		inline BpSceneDesc();

	public:

		BpVec3 gravity;

		//Contact with relative velocity below this will not bounce. Try 0.2 * gravity
		float bounceThresholdVelocity;
	};

	class BpScene
	{
	public:
		BpScene();
		~BpScene();

		void Release();

		void InitFromSceneDesc(const BpSceneDesc& sceneDesc);

		void SetGravity(const BpVec3& vec) { m_gravity = vec; }
		inline BpVec3 GetGravity() { return m_gravity; }

		void SetBounceThresholdVelocity(const float f) { m_bounceThresholdVelocity = f; }
		float GetBounceThresholdVelocity() { return m_bounceThresholdVelocity; }

		//Add Actor to proper store based off type
		void AddActor(BpActor* actor);
		
		void RemoveActor(BpActor* actor);

		//Simulate the scene forward by the given timestep
		void Simulate(float timestep);

		//Debug Visualization
		void SetVisualizationParameter(BpVisualizationParams parameter, float newScale);
		float GetVisualizationParameter(BpVisualizationParams parameter);

	private:
		BpVec3 m_gravity;
		float m_bounceThresholdVelocity;

		std::vector<BpRigidDynamic*> m_sceneRigidDynamics;

		BpVisualizationManager* m_pVisualizationManager;
	};

	inline BpSceneDesc::BpSceneDesc()
		: gravity(BpVec3()),
		bounceThresholdVelocity(2)
	{
	}
}