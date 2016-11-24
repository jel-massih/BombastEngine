#pragma once

#include "Physics.h"
#include "BpPhysicsApi.h"

using namespace bPhysics;


class BombastDebugPhysicsSphere : public IDebugPhysicsShape
{
public:
	BombastDebugPhysicsSphere(Mat4x4 transform, Vec3 color, float radius) : m_transform(transform), m_color(color), m_radius(radius) {}

	virtual Vec3 VGetColor() { return m_color; }
	virtual DebugPhysicsShapeType VGetShapeType() { return DebugPhysicsShapeType::SPHERE; }
	float VGetRadius() { return m_radius; }

	virtual Mat4x4 VGetTransform() override { return m_transform; }

private:
	Vec3 m_color;
	float m_radius;
	Mat4x4 m_transform;
};

class BombastDebugPhysicsBox : public IDebugPhysicsShape
{
public:
	BombastDebugPhysicsBox(Mat4x4 transform, Vec3 color, Vec3 extent) : m_transform(transform), m_color(color), m_extent(extent) {}

	virtual Vec3 VGetColor() { return m_color; }
	virtual DebugPhysicsShapeType VGetShapeType() { return DebugPhysicsShapeType::BOX; }
	Vec3 VGetExtent() { return m_extent; }

	virtual Mat4x4 VGetTransform() override { return m_transform; }

private:
	Vec3 m_color, m_extent;
	Mat4x4 m_transform;
};

class BombastDebugPhysicsCapsule : public IDebugPhysicsShape
{
public:
	BombastDebugPhysicsCapsule(Mat4x4 transform, Vec3 color, float radius, float halfHeight) : m_transform(transform), m_color(color), m_radius(radius), m_halfHeight(halfHeight) {}

	virtual Vec3 VGetColor() { return m_color; }
	virtual DebugPhysicsShapeType VGetShapeType() { return DebugPhysicsShapeType::CAPSULE; }
	float VGetRadius() { return m_radius; }
	float VGetHalfHeight() { return m_halfHeight; }

	virtual Mat4x4 VGetTransform() override { return m_transform; }

private:
	Vec3 m_color;
	float m_radius, m_halfHeight;
	Mat4x4 m_transform;
};

class BombastPhysicsDebugRenderBuffer : public IDebugPhysicsRenderBuffer
{
public:
	~BombastPhysicsDebugRenderBuffer();

	virtual std::vector<IDebugPhysicsShape*> VGetDebugShapes() { return m_shapes; }
	
	std::vector<IDebugPhysicsShape*> m_shapes;
};

class BombastPhysics : public IGamePhysics, BE_NonCopyable
{
	typedef std::map<std::string, float> DensityTable;
	typedef std::map<std::string, PhysicsMaterialData> MaterialTable;

	typedef std::map<ActorId, BpRigidBody*> ActorIdToBPRigidBodyTable;
	typedef std::map<BpRigidBody const *, ActorId> BPRigidBodyToActorIdTable;

public:
	BombastPhysics();
	virtual ~BombastPhysics();

	virtual bool VInitialize() override;
	virtual void VSyncVisibleScene() override;
	virtual void VOnUpdate(const float deltaMs) override;

	virtual void VAddSphere(float radius, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform) override;
	virtual void VAddBox(Vec3 scale, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform) override;
	virtual void VAddCapsule(float radius, float halfHeight, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform) override;
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

private:
	void VLoadPhysicsConfigXml();

	PhysicsMaterialData LookupMaterialData(const std::string& materialString);
	float LookupDensity(const std::string& densityString);

	void AddShape(Actor* pActor, BpGeometry* geometry, float density, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform);

	void RemovePhysicsObject(BpRigidBody* body);

	BpRigidBody* FindRigidBody(ActorId actorId);
	ActorId FindActorId(BpRigidBody const * const body);

	void Mat4x4ToBpMat4x4(const Mat4x4& input, BpMat4x4* output);
	void BpMat4x4ToMat4x4(const BpMat4x4& input, Mat4x4* output);
	void BpVec3ToVec3(const BpVec3& input, Vec3* output);

	IDebugPhysicsShape* ConvertPhysicsDebugShape(BpDebugShape* shape);

private:
	static const float Timestep; //Timestep value
	
	BpPhysicsCore* m_pPhysicsCore;
	BpFoundation* m_pFoundation;

	BpScene* m_pScene;

	DensityTable m_densityTable;
	MaterialTable m_materialTable;
	ActorIdToBPRigidBodyTable m_actorRigidBodyMap;
	BPRigidBodyToActorIdTable m_rigidBodyActorMap;
};