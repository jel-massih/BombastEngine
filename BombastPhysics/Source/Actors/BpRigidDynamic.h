#pragma once

#include "BpRigidBody.h"

namespace bPhysics
{
	//BpRigidDynamic represents dynamic rigid simulation
	//Created by calling BpPhysicsCore::CreateRigidDynamic
	class BpRigidDynamic : public BpRigidBody
	{
	public:
		BpRigidDynamic();
		~BpRigidDynamic();

		inline virtual void SetLinearDamping(float linearDamping) { m_linearDamping = linearDamping; }
		inline virtual float GetLinearDamping() const { return m_linearDamping; }

		inline virtual void SetAngularDamping(float angularDamping) { m_angularDamping = angularDamping; }
		inline virtual float GetAngularDamping() const { return m_angularDamping; }

		inline virtual void SetMaxAngularVelocity(float maxAngularVel) { m_maxAngularVelocity = maxAngularVel; }
		inline virtual float GetMaxAngularVelocity() const { return m_maxAngularVelocity; }

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

	private:
		BpVec3 m_velocity, m_angularVelocity;

		float m_linearDamping, m_angularDamping, m_maxAngularVelocity;

		float m_sleepThreshold, m_stabilizationThreshold;

		unsigned int m_wakeCounter, m_resetWakeCounterValue;

		bool m_bSleeping;
	};
}