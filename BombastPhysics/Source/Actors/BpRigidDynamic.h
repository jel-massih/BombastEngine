#pragma once

#include "BpRigidBody.h"
#include "../Geometry/BpShape.h"

namespace bPhysics
{
	class BpShape;

	//BpRigidDynamic represents dynamic rigid simulation
	//Created by calling BpPhysicsCore::CreateRigidDynamic
	class BpRigidDynamic : public BpRigidBody
	{
	public:
		BpRigidDynamic(const BpShape shape);
		~BpRigidDynamic();

		//Retrieves actors world space transform
		inline virtual BpMat4x4 GetWorldTransform() const;
		inline virtual void SetWorldTransform(const BpMat4x4& newTransform, bool autoWake = true);

		virtual BpActorType::Type GetType() const { return BpActorType::RIGID_DYNAMIC; }

		virtual void SetName(const char* name) { m_name = name; }
		virtual const char* GetName() const { return m_name; }

		virtual BpBounds3 GetWorldBounds(float inflation = 1.01f) const;

		inline virtual void SetLinearDamping(float linearDamping) { m_linearDamping = linearDamping; }
		inline virtual float GetLinearDamping() const { return m_linearDamping; }

		inline virtual void SetAngularDamping(float angularDamping) { m_angularDamping = angularDamping; }
		inline virtual float GetAngularDamping() const { return m_angularDamping; }

		inline virtual void SetMaxAngularVelocity(float maxAngularVel) { m_maxAngularVelocity = maxAngularVel; }
		inline virtual float GetMaxAngularVelocity() const { return m_maxAngularVelocity; }

		inline virtual void SetGravityEnabled(bool gravityEnabled) { m_gravityEnabled = gravityEnabled; }
		inline virtual float GetGravityEnabled() const { return m_gravityEnabled; }

		//True if body sleeping.
		//When not moving for period of time, body no longer simulated [enters sleep]
		//Gauranteed to be awake if: Wake Counter Positive, velocity non-zero, nonzero force applied
		//Gauranteed to be Asleep if: Wake Counter Zero, veloicty zero, no force
		inline virtual bool IsSleeping() const { return m_bSleeping; }

		//Mass-normalized kinetic energy threshold below which actor may be asleep
		inline virtual void SetSleepThreshold(float threshold) { m_sleepThreshold = threshold; }
		inline virtual float GetSleepThreshold() const { return m_sleepThreshold; }

		//Mass-Normalized Kenetic Energey threshold below which actor may be stabilized
		inline virtual void SetStabilizationThreshold(float threshold) { m_stabilizationThreshold = threshold; }
		inline virtual float GetStabilizationThreshold() const { return m_stabilizationThreshold; }

		//Minimum amount of time until body can be put to sleep
		inline virtual void SetWakeCounter(unsigned int val) { m_wakeCounter = val; }
		inline virtual unsigned int GetWakeCounter() const { return m_wakeCounter; }

		//Wake actor if sleeping
		virtual void WakeUp();

		//Force actor to sleep
		virtual void ForceSleep();

		inline void SetScene(BpScene* scene) { m_pScene = scene; }
		inline BpScene const * GetScene() const { return m_pScene; }

		void Simulate(float timestep);

	private:
		BpVec3 m_velocity, m_angularVelocity;

		BpShape m_shape;

		BpScene* m_pScene;

		float m_linearDamping, m_angularDamping, m_maxAngularVelocity;

		bool m_gravityEnabled;

		float m_sleepThreshold, m_stabilizationThreshold;

		unsigned int m_wakeCounter, m_resetWakeCounterValue;

		const char* m_name;

		bool m_bSleeping;
	};
}