#pragma once

#include "Physics.h"
#include <PxPhysicsAPI.h>

using namespace physx;

class PhysXPhysics : public IGamePhysics, BE_NonCopyable
{
	typedef std::map<std::string, float> DensityTable;
	typedef std::map<std::string, PhysicsMaterialData> MaterialTable;

public:
	PhysXPhysics();
	virtual ~PhysXPhysics();

	virtual bool VInitialize() override;
	virtual void VSyncVisibleScene() override;
	virtual void VOnUpdate(float deltaSeconds) override;
	
	virtual void VAddSphere(float radius, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial) override;
	virtual void VAddBox(const Vec3& dimensions, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial) override;
	virtual void VRemoveActor(ActorId id) override;

	virtual void VRenderDiagnostics() override;

	virtual void VApplyForce(const Vec3 &dir, float newtons, ActorId actor) override;
	virtual void VApplyTorque(const Vec3 &dir, float newtons, ActorId actor) override;
	virtual bool VMove(const Mat4x4 &mat, ActorId aid) override;

	virtual void VStopActor(ActorId actorId);
	virtual Vec3 VGetVelocity(ActorId actorId);
	virtual void VSetVelocity(ActorId actorId, const Vec3& vel);
	virtual Vec3 VGetAngularVelocity(ActorId actorId);
	virtual void VSetAngularVelocity(ActorId actorId, const Vec3& vel);
	virtual void VTranslate(ActorId actorId, const Vec3& vec);

	virtual void VSetTransform(const ActorId id, const Mat4x4& mat) override;

	virtual Mat4x4 VGetTransform(const ActorId id);

private:
	void ConnectPVD();

private:
	PxPhysics* m_pPhysicsSdk;
	PxFoundation* m_pFoundation;
	PxDefaultErrorCallback m_errorCallback;
	PxDefaultAllocator m_allocatorCallback;
	PxDefaultCpuDispatcher* m_pDispatcher;
	PxVisualDebuggerConnection* m_pConnection;

	PxScene* m_pScene;
	static const PxReal Timestep; //Timestep value

	DensityTable m_densityTable;
	MaterialTable m_materialTable;
};