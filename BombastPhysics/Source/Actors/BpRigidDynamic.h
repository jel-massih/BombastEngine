#pragma once

#include "BpRigidBody.h"
#include "../Debugging/BpVisualizationManager.h"
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
		
		virtual BpBounds3 GetWorldBounds(f32 inflation = 1.01f) const;

		inline virtual void SetLinearDamping(f32 linearDamping) { m_linearDamping = linearDamping; }
		inline virtual f32 GetLinearDamping() const { return m_linearDamping; }

		inline virtual void SetAngularDamping(f32 angularDamping) { m_angularDamping = angularDamping; }
		inline virtual f32 GetAngularDamping() const { return m_angularDamping; }

		inline virtual void SetMaxAngularVelocity(f32 maxAngularVel) { m_maxAngularVelocity = maxAngularVel; }
		inline virtual f32 GetMaxAngularVelocity() const { return m_maxAngularVelocity; }

		inline virtual void SetGravityEnabled(bool gravityEnabled) { m_gravityEnabled = gravityEnabled; }
		inline virtual f32 GetGravityEnabled() const { return m_gravityEnabled; }

		//True if body sleeping.
		//When not moving for period of time, body no longer simulated [enters sleep]
		//Gauranteed to be awake if: Wake Counter Positive, velocity non-zero, nonzero force applied
		//Gauranteed to be Asleep if: Wake Counter Zero, veloicty zero, no force
		inline virtual bool IsSleeping() const { return m_bSleeping; }

		//Mass-normalized kinetic energy threshold below which actor may be asleep
		inline virtual void SetSleepThreshold(f32 threshold) { m_sleepThreshold = threshold; }
		inline virtual f32 GetSleepThreshold() const { return m_sleepThreshold; }

		//Mass-Normalized Kenetic Energey threshold below which actor may be stabilized
		inline virtual void SetStabilizationThreshold(f32 threshold) { m_stabilizationThreshold = threshold; }
		inline virtual f32 GetStabilizationThreshold() const { return m_stabilizationThreshold; }

		//Minimum amount of time until body can be put to sleep
		inline virtual void SetWakeCounter(u32 val) { m_wakeCounter = val; }
		inline virtual u32 GetWakeCounter() const { return m_wakeCounter; }

		//Wake actor if sleeping
		virtual void WakeUp();

		//Force actor to sleep
		virtual void ForceSleep();

		inline void SetScene(BpScene* scene) { m_pScene = scene; }
		inline BpScene * GetScene() const { return m_pScene; }

		void Simulate(f32 timestep);

		bool SetupBodyDebugging(BpVisualizationManager& visualizationManager);

		inline BpShape* GetShape() { return &m_shape; }

		void AddContact(BpContactEdge* contact) { m_contacts.push_back(contact); }

	private:
		BpVec3 m_velocity, m_angularVelocity;

		BpMat4x4 m_transform;

		BpShape m_shape;

		BpScene* m_pScene;

		std::vector<BpContactEdge*> m_contacts;

		f32 m_linearDamping, m_angularDamping, m_maxAngularVelocity;

		bool m_gravityEnabled;

		f32 m_sleepThreshold, m_stabilizationThreshold;

		u32 m_wakeCounter, m_resetWakeCounterValue;

		const char* m_name;

		bool m_bSleeping;
	};
}