#include "BpRigidDynamic.h"

#define BP_RIGID_DYNAMIC_DEFAULT_LINEAR_DAMPING (0.2f)
#define BP_RIGID_DYNAMIC_DEFAULT_ANGULAR_DAMPING (0.2f)
#define BP_RIGID_DYNAMIC_DEFAULT_MAX_ANGULAR_VELOCITY (10.f)
#define BP_RIGID_DYNAMIC_DEFAULT_STABILIZATION_THRESHOLD (0.1f)
#define BP_RIGID_DYNAMIC_DEFAULT_SLEEP_THRESHOLD (0.1f)
#define BP_RIGID_DYNAMIC_DEFAULT_RESET_WAKE_COUNTER_VALUE (10)

using namespace bPhysics;

BpRigidDynamic::BpRigidDynamic()
	: m_linearDamping(BP_RIGID_DYNAMIC_DEFAULT_LINEAR_DAMPING), m_angularDamping(BP_RIGID_DYNAMIC_DEFAULT_ANGULAR_DAMPING), m_maxAngularVelocity(BP_RIGID_DYNAMIC_DEFAULT_MAX_ANGULAR_VELOCITY),
	m_bSleeping(true), m_wakeCounter(0), m_resetWakeCounterValue(BP_RIGID_DYNAMIC_DEFAULT_RESET_WAKE_COUNTER_VALUE),
	m_stabilizationThreshold(BP_RIGID_DYNAMIC_DEFAULT_STABILIZATION_THRESHOLD), m_sleepThreshold(BP_RIGID_DYNAMIC_DEFAULT_SLEEP_THRESHOLD)
{
}

BpRigidDynamic::~BpRigidDynamic()
{

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