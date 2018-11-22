#include "StubPhysics.h"

StubPhysics::StubPhysics()
{
}

StubPhysics::~StubPhysics()
{
}

bool StubPhysics::VInitialize()
{
	return true;
}

void StubPhysics::VLoadPhysicsConfigXml()
{
}

void StubPhysics::VOnUpdate(const float deltaMs)
{
}

void StubPhysics::VSyncVisibleScene()
{
}

void StubPhysics::VAddSphere(float radius, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform, const std::string& bodyType)
{
}

void StubPhysics::VAddBox(Vec3 scale, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform, const std::string& bodyType)
{
}

void StubPhysics::VAddCapsule(float radius, float halfHeight, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform, const std::string& bodyType)
{
}

void StubPhysics::VRemoveActor(ActorId id)
{
}

void StubPhysics::VSetDebugVisualizationEnabled(bool enabled)
{
}

void StubPhysics::VRenderDiagnostics()
{
}

void StubPhysics::VApplyForce(const Vec3 &dir, float newtons, ActorId actorId)
{
}

void StubPhysics::VApplyTorque(const Vec3 &dir, float newtons, ActorId actorId)
{
}

bool StubPhysics::VMove(const Mat4x4 &mat, ActorId actorId)
{
	return true;
}

void StubPhysics::VStopActor(ActorId actorId)
{
}

Vec3 StubPhysics::VGetVelocity(ActorId actorId)
{
	return Vec3::g_IdentityVec3;
}

void StubPhysics::VSetVelocity(ActorId actorId, const Vec3& vel)
{
}

Vec3 StubPhysics::VGetAngularVelocity(ActorId actorId)
{
	return Vec3::g_IdentityVec3;
}

void StubPhysics::VSetAngularVelocity(ActorId actorId, const Vec3& vel)
{
}

void StubPhysics::VTranslate(ActorId actorId, const Vec3& vec)
{
}

void StubPhysics::VSetTransform(const ActorId id, const Mat4x4& mat)
{
}

Mat4x4 StubPhysics::VGetTransform(const ActorId id)
{
	return Mat4x4::g_Identity;
}

IDebugPhysicsRenderBuffer * StubPhysics::VGetDebugRenderBuffer()
{
	return nullptr;
}