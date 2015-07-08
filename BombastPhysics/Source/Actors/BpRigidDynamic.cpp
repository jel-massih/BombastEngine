#include "BpRigidDynamic.h"
#include "../Scene/BpScene.h"

#define BP_RIGID_DYNAMIC_DEFAULT_LINEAR_DAMPING (0.2f)
#define BP_RIGID_DYNAMIC_DEFAULT_ANGULAR_DAMPING (0.2f)
#define BP_RIGID_DYNAMIC_DEFAULT_MAX_ANGULAR_VELOCITY (10.f)
#define BP_RIGID_DYNAMIC_DEFAULT_STABILIZATION_THRESHOLD (0.1f)
#define BP_RIGID_DYNAMIC_DEFAULT_SLEEP_THRESHOLD (0.1f)
#define BP_RIGID_DYNAMIC_DEFAULT_RESET_WAKE_COUNTER_VALUE (10)

using namespace bPhysics;

BpRigidDynamic::BpRigidDynamic(const BpShape shape)
	: m_linearDamping(BP_RIGID_DYNAMIC_DEFAULT_LINEAR_DAMPING), m_angularDamping(BP_RIGID_DYNAMIC_DEFAULT_ANGULAR_DAMPING), m_maxAngularVelocity(BP_RIGID_DYNAMIC_DEFAULT_MAX_ANGULAR_VELOCITY),
	m_bSleeping(true), m_wakeCounter(0), m_resetWakeCounterValue(BP_RIGID_DYNAMIC_DEFAULT_RESET_WAKE_COUNTER_VALUE),
	m_stabilizationThreshold(BP_RIGID_DYNAMIC_DEFAULT_STABILIZATION_THRESHOLD), m_sleepThreshold(BP_RIGID_DYNAMIC_DEFAULT_SLEEP_THRESHOLD),
	m_shape(shape)
{
	m_shape.SetOwner(this);
}

BpRigidDynamic::~BpRigidDynamic()
{
}

inline BpMat4x4 BpRigidDynamic::GetWorldTransform() const
{
	return m_transform;
}

inline void BpRigidDynamic::SetWorldTransform(const BpMat4x4& newTransform, bool autoWake)
{
	m_transform = newTransform;

	if (autoWake)
	{
		WakeUp();
	}
}

BpBounds3 BpRigidDynamic::GetWorldBounds(f32 inflation) const
{
	throw "Not Implemented Exception";
	BpBounds3 bounds;
	bounds.SetEmpty();

	return bounds;
}

void BpRigidDynamic::WakeUp()
{
	m_wakeCounter = m_resetWakeCounterValue;
	m_bSleeping = false;
}

void BpRigidDynamic::ForceSleep()
{
	m_wakeCounter = 0;
	m_velocity = BpVec3(0, 0, 0);
	m_angularVelocity = BpVec3(0, 0, 0);
	m_bSleeping = true;
}

void BpRigidDynamic::Simulate(f32 timestep)
{
	BpVec3 acceleration;

	if (m_gravityEnabled)
	{
		acceleration = m_pScene->GetGravity();
	}

	m_velocity = m_velocity + (acceleration * timestep);

	if (m_velocity.Length() > m_sleepThreshold || acceleration.Length() > m_sleepThreshold)
	{
		WakeUp();

		BpVec3 pos = m_transform.GetPosition();
		pos += (m_velocity * timestep);
		m_transform.SetPosition(pos);
	}
	else
	{
		m_wakeCounter--;

		if (m_wakeCounter <= 0) {
			ForceSleep();
		}
	}
}

bool BpRigidDynamic::SetupBodyDebugging(BpVisualizationManager& visualizationManager)
{
	visualizationManager.VisualizeShape(m_shape, *this);

	return true;
}