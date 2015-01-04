#include "PhysXPhysics.h"

#define ENABLE_PHYSX_PVD true

const PxReal PhysXPhysics::Timestep = 1.00f / 60.0f;

PhysXPhysics::PhysXPhysics()
{

}

PhysXPhysics::~PhysXPhysics()
{
	m_pPhysicsSdk->release();
	m_pFoundation->release();
}

bool PhysXPhysics::VInitialize()
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocatorCallback, m_errorCallback);
	m_pPhysicsSdk = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale());

	if (!m_pPhysicsSdk)
	{
		BE_ERROR("Error Creating PhysX device.");
		return false;
	}

	PxSceneDesc sceneDesc(m_pPhysicsSdk->getTolerancesScale());

	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f); //Set Gravity
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	m_pScene = m_pPhysicsSdk->createScene(sceneDesc);

#ifdef ENABLE_PHYSX_PVD
	ConnectPVD();
#endif

	return true;
}

void PhysXPhysics::ConnectPVD()
{
	//Check if PVD manager is available
	if (m_pPhysicsSdk->getPvdConnectionManager() == NULL)
	{
		return;
	}

	const char* pvd_host_ip = "127.0.0.1"; //IP of PVD PC
	int port = 5425; //TCP port
	unsigned int timeout = 100; //Timeout in ms

	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

	//Try to establish connection
	debugger::comm::PvdConnection* pvdConnection = PxVisualDebuggerExt::createConnection(m_pPhysicsSdk->getPvdConnectionManager(), pvd_host_ip, port, timeout, connectionFlags);
}

void PhysXPhysics::VOnUpdate(float const deltaSeconds)
{
	m_pScene->simulate(PhysXPhysics::Timestep);
	m_pScene->fetchResults(true);
}

void PhysXPhysics::VSyncVisibleScene()
{
	//@TODO: Sync Graphics to Physics?
	//throw "Function not yet implemented.";
}

void PhysXPhysics::VAddSphere(float radius, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial)
{
	throw "Function not yet implemented.";
}

void PhysXPhysics::VAddBox(const Vec3& dimensions, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial)
{
	throw "Function not yet implemented.";
}

void PhysXPhysics::VRemoveActor(ActorId id)
{
	throw "Function not yet implemented.";
}

void PhysXPhysics::VRenderDiagnostics()
{
	throw "Function not yet implemented.";
}

void PhysXPhysics::VApplyForce(const Vec3 &dir, float newtons, ActorId actor)
{
	throw "Function not yet implemented.";
}

void PhysXPhysics::VApplyTorque(const Vec3 &dir, float newtons, ActorId actor)
{
	throw "Function not yet implemented.";
}

bool PhysXPhysics::VMove(const Mat4x4 &mat, ActorId aid)
{
	throw "Function not yet implemented.";
	return true;
}

void PhysXPhysics::VStopActor(ActorId actorId)
{
	throw "Function not yet implemented.";
}

Vec3 PhysXPhysics::VGetVelocity(ActorId actorId)
{
	throw "Function not yet implemented.";
	return Vec3::g_InvalidVec3;
}

void PhysXPhysics::VSetVelocity(ActorId actorId, const Vec3& vel)
{
	throw "Function not yet implemented.";
}

Vec3 PhysXPhysics::VGetAngularVelocity(ActorId actorId)
{
	throw "Function not yet implemented.";
	return Vec3::g_InvalidVec3;
}

void PhysXPhysics::VSetAngularVelocity(ActorId actorId, const Vec3& vel)
{
	throw "Function not yet implemented.";
}

void PhysXPhysics::VTranslate(ActorId actorId, const Vec3& vec)
{
	throw "Function not yet implemented.";
}

void PhysXPhysics::VSetTransform(const ActorId id, const Mat4x4& mat)
{
	throw "Function not yet implemented.";
}

Mat4x4 PhysXPhysics::VGetTransform(const ActorId id)
{
	throw "Function not yet implemented.";
	return Mat4x4::g_Identity;
}