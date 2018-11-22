#pragma once

#include "Physics.h"

class StubPhysics : public IGamePhysics, BE_NonCopyable
{
public:
	StubPhysics();
	virtual ~StubPhysics();

	virtual bool VInitialize() override;
	virtual void VSyncVisibleScene() override;
	virtual void VOnUpdate(const float deltaMs) override;

	virtual void VAddSphere(float radius, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform, const std::string& bodyType) override;
	virtual void VAddBox(Vec3 scale, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform, const std::string& bodyType) override;
	virtual void VAddCapsule(float radius, float halfHeight, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform, const std::string& bodyType) override;
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

	virtual IDebugPhysicsRenderBuffer* VGetDebugRenderBuffer();

protected:
	virtual void VLoadPhysicsConfigXml();

};
