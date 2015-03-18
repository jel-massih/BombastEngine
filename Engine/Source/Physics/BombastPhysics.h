#pragma once

#include "Physics.h"
#include "BpPhysicsApi.h"

using namespace bPhysics;

class BombastPhysics : public IGamePhysics, BE_NonCopyable
{
	typedef std::map<std::string, float> DensityTable;
	typedef std::map<std::string, PhysicsMaterialData> MaterialTable;

public:
	BombastPhysics();
	virtual ~BombastPhysics();

	virtual bool VInitialize() override;
	virtual void VSyncVisibleScene() override;
	virtual void VOnUpdate(float deltaMs) override;

	virtual void VAddSphere(float radius, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping) override;
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
	void VLoadPhysicsConfigXml();

	PhysicsMaterialData LookupMaterialData(const std::string& materialString);
	float LookupDensity(const std::string& densityString);

	void ConnectPVD();

	void Mat4x4ToBpMat4x4(const Mat4x4& input, BpMat4x4* output);
	void BpMat4x4ToMat4x4(const BpMat4x4& input, Mat4x4* output);
	void BpVec3ToVec3(const BpVec3& input, Vec3* output);

private:
	static const float Timestep; //Timestep value
	
	BpPhysicsCore* m_pPhysicsCore;
	BpFoundation* m_pFoundation;

	BpScene* m_pScene;

	DensityTable m_densityTable;
	MaterialTable m_materialTable;
};