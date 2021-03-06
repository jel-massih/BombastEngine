#pragma once
#ifdef USE_PHYSX

#if defined(_DEBUG)
#pragma comment(lib, "PhysX3DEBUG_x86.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib")
#else
#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PhysX3Common_x86.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PhysXVisualDebuggerSDK.lib")
#endif

/*
PhysX3_x64.lib
PhysX3Common_x64.lib
PhysX3Extensions.lib
PhysXVisualDebuggerSDK.lib
*/

#include "Physics.h"
#include <PxPhysicsAPI.h>

using namespace physx;

class BombastPhysXErrorCallback : public PxErrorCallback
{
public:
	virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		BE_ERROR(message);
	}
};

class PhysXPhysicsDebugRenderBuffer : public IDebugPhysicsRenderBuffer
{
public:
	~PhysXPhysicsDebugRenderBuffer();

	virtual std::vector<IDebugPhysicsShape*> VGetDebugShapes() { return m_shapes; }

	std::vector<IDebugPhysicsShape*> m_shapes;
};

class PhysXPhysics : public IGamePhysics, BE_NonCopyable
{
	typedef std::map<std::string, float> DensityTable;
	typedef std::map<std::string, PhysicsMaterialData> MaterialTable;
	
	typedef std::map<ActorId, PxRigidBody*> ActorIdToPysXRigidBodyTable;
	typedef std::map<PxRigidBody const *, ActorId> PysXRigidBodyToActorIdTable;

public:
	PhysXPhysics();
	virtual ~PhysXPhysics();

	virtual bool VInitialize() override;
	virtual void VSyncVisibleScene() override;
	virtual void VOnUpdate(const float deltaMs) override;
	
	virtual void VAddSphere(float radius, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial,
		bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform, const std::string& bodyType) override;
	virtual void VAddBox(Vec3 scale, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial,
		bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform, const std::string& bodyType) override;
	virtual void VAddCapsule(float radius, float halfHeight, Actor* gameActor, const std::string& densityStr, 
		const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform, const std::string& bodyType) override;
	virtual void VRemoveActor(ActorId id) override;

	virtual void VSetDebugVisualizationEnabled(bool bEnabled) override;
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

	virtual IDebugPhysicsRenderBuffer* VGetDebugRenderBuffer() override;

private:
	void AddShape(Actor* pActor, PxGeometry* geometry, float density, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, const std::string& bodyType);
	void RemovePhysicsObject(PxRigidBody* body);

	void VLoadPhysicsConfigXml();

	PhysicsMaterialData LookupMaterialData(const std::string& materialString);
	float LookupDensity(const std::string& densityString);
	
	PxRigidBody* FindRigidBody(ActorId actorId);
	ActorId FindActorId(PxRigidBody const * const body);

	void ConnectPVD();

	void Mat4x4ToPxMatrix(const Mat4x4& input, PxMat44* output);
	void PxMatrixToMat4x4(const PxMat44& input, Mat4x4* output);
	void Vec3ToPxVec(const Vec3& input, PxVec3* output);
	void PxVecToVec3(const PxVec3& input, Vec3* output);
	void QuaternionToPxQuat(const Quaternion& input, PxQuat* output);
	void PxQuatToQuaternion(const PxQuat& input, Quaternion* output);

private:
	PxPhysics* m_pPhysicsSdk;
	PxFoundation* m_pFoundation;
	BombastPhysXErrorCallback m_errorCallback;
	PxDefaultAllocator m_allocatorCallback;
	PxDefaultCpuDispatcher* m_pDispatcher;
	PxVisualDebuggerConnection* m_pConnection;

	PxScene* m_pScene;
	static const PxReal Timestep; //Timestep value

	DensityTable m_densityTable;
	MaterialTable m_materialTable;
	ActorIdToPysXRigidBodyTable m_actorRigidBodyMap;
	PysXRigidBodyToActorIdTable m_rigidBodyActorMap;
};
#endif