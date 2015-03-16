#pragma once

#include "../Utils/BpMath.h"

namespace bPhysics
{
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

		void InitFromSceneDesc(const BpSceneDesc& sceneDesc);

		void SetGravity(const BpVec3& vec) { m_gravity = vec; }
		inline BpVec3 GetGravity() { return m_gravity; }

		void SetBounceThresholdVelocity(const float f) { m_bounceThresholdVelocity = f; }
		float GetBounceThresholdVelocity() { return m_bounceThresholdVelocity; }

	private:
		BpVec3 m_gravity;
		float m_bounceThresholdVelocity;
	};

	inline BpSceneDesc::BpSceneDesc()
		: gravity(BpVec3()),
		bounceThresholdVelocity(2)
	{
	}
}