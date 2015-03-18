#pragma once

#include "BpRigidBody.h"

namespace bPhysics
{
	//BpRigidDynamic represents dynamic rigid simulation
	//Created by calling BpPhysicsCore::CreateRigidDynamic
	class BpRigidDynamic : public BpRigidBody
	{
	public:
		virtual void SetLinearDamping(float linearDamping) = 0;
		virtual float GetLinearDamping() const = 0;

		virtual void SetAngularDamping(float angularDamping) = 0;
		virtual float GetAngularDamping() const = 0;

		virtual void SetMaxAngularVelocity(float maxAngularVel) = 0;
		virtual float GetMaxAngularVelocity() const = 0;

		//True if body sleeping.
		//When not moving for period of time, body no longer simulated [enters sleep]
		//Gauranteed to be awake if: Wake Counter Positive, velocity non-zero, nonzero force applied
		//Gauranteed to be Asleep if: Wake Counter Zero, veloicty zero, no force
		virtual bool IsSleeping() const = 0;

		//Mass-normalized kinetic energy threshold below which actor may be asleep
		virtual void SetSleepThreshold(float threshold) = 0;
		virtual float GetSleepThreshold() const = 0;

		//Mass-Normalized Kenetic Energey threshold below which actor may be stabilized
		virtual void SetStabilizationThreshold(float threshold) = 0;
		virtual float GetStabilizationThreshold() const = 0;

		//Minimum amount of time until body can be put to sleep
		virtual void SetWakeCounter(float val) = 0;
		virtual float GetWakeCounter() const = 0;

		//Wake actor if sleeping
		virtual void WakeUp() = 0;

		//Force actor to sleep
		virtual void ForceSleep() = 0;

	protected:
		inline BpRigidDynamic() {}
		virtual ~BpRigidDynamic() {}
	};
}