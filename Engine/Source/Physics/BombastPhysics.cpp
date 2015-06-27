#include "BombastPhysics.h"
#include "../Actor/TransformComponent.h"
#include "../Resources/XmlResource.h"
#include "../Events/Events.h"

const float BombastPhysics::Timestep = 1.00f / 60.0f;

const float BE_PHYSICS_GRAVITY = -9.91f;

BombastPhysics::BombastPhysics()
	: m_pFoundation(nullptr), m_pPhysicsCore(nullptr)
{
}

BombastPhysics::~BombastPhysics()
{
	SAFE_DELETE(m_pFoundation);
	SAFE_DELETE(m_pPhysicsCore);

	SAFE_RELEASE(m_pScene);
}

bool BombastPhysics::VInitialize()
{
	VLoadPhysicsConfigXml();

	m_pFoundation = BE_NEW BpFoundation;
	m_pPhysicsCore = BE_NEW BpPhysicsCore;

	if (!m_pPhysicsCore) 
	{
		BE_ERROR("Error Creating Bombast Physics Device");
		return false;
	}

	BpSceneDesc sceneDesc;
	sceneDesc.gravity = BpVec3(0.0f, -9.91f, 0.0f);
	sceneDesc.bounceThresholdVelocity = BE_PHYSICS_GRAVITY * -0.2f;

	m_pScene = m_pPhysicsCore->CreateScene(sceneDesc);

	return true;
}

PhysicsMaterialData BombastPhysics::LookupMaterialData(const std::string& materialString)
{
	auto it = m_materialTable.find(materialString);
	if (it != m_materialTable.end())
	{
		return it->second;
	}
	else
	{
		return PhysicsMaterialData(0, 0);
	}
}

float BombastPhysics::LookupDensity(const std::string& densityString)
{
	float density = 1.0f;
	auto it = m_densityTable.find(densityString);
	if (it != m_densityTable.end())
	{
		density = it->second;
	}

	return density;
}

void BombastPhysics::VLoadPhysicsConfigXml()
{
	rapidxml::xml_node<>* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement("Config\\physicsConfig.xml");
	BE_ASSERT(pRoot);

	rapidxml::xml_node<>* pMatNode = pRoot->first_node("PhysicsMaterials");
	BE_ASSERT(pMatNode);
	for (rapidxml::xml_node<>* pNode = pMatNode->first_node(); pNode; pNode = pNode->next_sibling())
	{
		double restitution = 0;
		double friction = 0;
		restitution = atof(pNode->first_attribute("restitution")->value());
		friction = atof(pNode->first_attribute("friction")->value());
		m_materialTable.insert(std::make_pair(pNode->name(), PhysicsMaterialData((float)restitution, (float)friction)));
	}

	rapidxml::xml_node<>* pDensityNode = pRoot->first_node("DensityTable");
	BE_ASSERT(pDensityNode);
	for (rapidxml::xml_node<>* pNode = pDensityNode->first_node(); pNode; pNode = pNode->next_sibling())
	{
		m_densityTable.insert(std::make_pair(pNode->name(), (float)atof(pNode->value())));
	}
}

void BombastPhysics::VOnUpdate(float const deltaMs)
{
	m_pScene->Simulate(BombastPhysics::Timestep);
}

void BombastPhysics::VSyncVisibleScene()
{
	for (ActorIdToBPRigidBodyTable::const_iterator it = m_actorRigidBodyMap.begin(); it != m_actorRigidBodyMap.end(); it++)
	{
		ActorId const id = it->first;

		BpMat4x4 bpLoc = it->second->GetWorldTransform();
		Mat4x4 loc;
		BpMat4x4ToMat4x4(bpLoc, &loc);

		Actor* pActor = g_pApp->m_pGame->VGetActor(id);
		if (pActor)
		{
			TransformComponent* pTransformComponent = pActor->GetComponent<TransformComponent>(TransformComponent::g_Name);

			if (pTransformComponent)
			{
				if (pTransformComponent->GetTransform() != loc)
				{
					pTransformComponent->SetTransform(loc);
					IEventDataPtr pEvent(BE_NEW EvtData_Move_Actor(id, loc));
					IEventManager::Get()->VQueueEvent(pEvent);
				}
			}
		}
	}
}

void BombastPhysics::VAddSphere(float radius, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping)
{
	float density = LookupDensity(densityStr);

	AddShape(gameActor, &BpGeometrySphere(radius), density, physicsMaterial, gravityEnabled, linearDamping, angularDamping);
}

void BombastPhysics::VAddBox(Vec3 scale, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping)
{
	float density = LookupDensity(densityStr);

	AddShape(gameActor, &BpGeometryBox(scale.x, scale.y, scale.z), density, physicsMaterial, gravityEnabled, linearDamping, angularDamping);
}

void BombastPhysics::VRemoveActor(ActorId id)
{
	if (BpRigidBody* const body = FindRigidBody(id))
	{
		RemovePhysicsObject(body);
		m_actorRigidBodyMap.erase(id);
		m_rigidBodyActorMap.erase(body);
	}
}

void BombastPhysics::VRenderDiagnostics()
{
	throw "Function not yet implemented.";
}

void BombastPhysics::VApplyForce(const Vec3 &dir, float newtons, ActorId actorId)
{
	throw "Function not yet implemented.";
}

void BombastPhysics::VApplyTorque(const Vec3 &dir, float newtons, ActorId actorId)
{
	throw "Function not yet implemented.";
}

bool BombastPhysics::VMove(const Mat4x4 &mat, ActorId actorId)
{
	throw "Function not yet implemented.";
	return false;
}

void BombastPhysics::VStopActor(ActorId actorId)
{
	throw "Function not yet implemented.";
}

Vec3 BombastPhysics::VGetVelocity(ActorId actorId)
{
	throw "Function not yet implemented.";
	return Vec3::g_InvalidVec3;
}

void BombastPhysics::VSetVelocity(ActorId actorId, const Vec3& vel)
{
	throw "Function not yet implemented.";
}

Vec3 BombastPhysics::VGetAngularVelocity(ActorId actorId)
{
	throw "Function not yet implemented.";
	return Vec3::g_InvalidVec3;
}

void BombastPhysics::VSetAngularVelocity(ActorId actorId, const Vec3& vel)
{
	throw "Function not yet implemented.";
}

void BombastPhysics::VTranslate(ActorId actorId, const Vec3& vec)
{
	throw "Function not yet implemented.";
}

void BombastPhysics::VSetTransform(const ActorId id, const Mat4x4& mat)
{
	throw "Function not yet implemented.";
}

Mat4x4 BombastPhysics::VGetTransform(const ActorId id)
{
	throw "Function not yet implemented.";
	return Mat4x4::g_Identity;
}

void BombastPhysics::AddShape(Actor* pActor, BpGeometry* geometry, float density, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping)
{
	BE_ASSERT(pActor);
	BE_ASSERT(geometry);

	ActorId actorId = pActor->GetId();
	BE_ASSERTf(m_actorRigidBodyMap.find(actorId) == m_actorRigidBodyMap.end(), "Actor with more than one rigidbody");

	Mat4x4 transform = Mat4x4::g_Identity;
	TransformComponent* pTransformComponent = pActor->GetComponent<TransformComponent>(TransformComponent::g_Name);

	if (pTransformComponent)
	{
		transform = pTransformComponent->GetTransform();
	}
	else
	{
		BE_ERROR("BombastPhysics::AddShape Requires a transform");
		return;
	}

	BE_LOG_PHYSICS("Adding Shape of type %d to Actor: %d", geometry, actorId);

	PhysicsMaterialData material(LookupMaterialData(physicsMaterial));
	BpMaterial* mat = m_pPhysicsCore->CreateMaterial(material.m_friction, material.m_friction, material.m_restitution);

	BpMat4x4 bpMat;
	Mat4x4ToBpMat4x4(transform, &bpMat);

	BpRigidDynamic* body = BpCreateDynamic(*m_pPhysicsCore, bpMat, *geometry, *mat, density);

	if (!body)
	{
		BE_ERROR("BombastPhysics::AddShape Failed to create Dynamic RigidBody");
		return;
	}
	
	body->SetLinearDamping(linearDamping);
	body->SetAngularDamping(angularDamping);
	body->SetGravityEnabled(gravityEnabled);

	m_pScene->AddActor(body);
	m_actorRigidBodyMap[actorId] = body;
	m_rigidBodyActorMap[body] = actorId;

	SAFE_DELETE(mat);
}

void BombastPhysics::RemovePhysicsObject(BpRigidBody* body)
{
	m_pScene->RemoveActor(body);
}

BpRigidBody* BombastPhysics::FindRigidBody(ActorId actorId)
{
	ActorIdToBPRigidBodyTable::const_iterator found = m_actorRigidBodyMap.find(actorId);
	if (found != m_actorRigidBodyMap.end())
	{
		return found->second;
	}

	return NULL;
}

ActorId BombastPhysics::FindActorId(BpRigidBody const * const body)
{
	BPRigidBodyToActorIdTable::const_iterator found = m_rigidBodyActorMap.find(body);
	if (found != m_rigidBodyActorMap.end())
	{
		return found->second;
	}

	return ActorId();
}

void BombastPhysics::Mat4x4ToBpMat4x4(const Mat4x4& input, BpMat4x4* output)
{
	output->col0.x = input._11;
	output->col0.y = input._12;
	output->col0.z = input._13;
	output->col0.w = input._14;

	output->col1.x = input._21;
	output->col1.y = input._22;
	output->col1.z = input._23;
	output->col1.w = input._24;

	output->col2.x = input._31;
	output->col2.y = input._32;
	output->col2.z = input._33;
	output->col2.w = input._34;

	output->col3.x = input._41;
	output->col3.y = input._42;
	output->col3.z = input._43;
	output->col3.w = input._44;
}

void BombastPhysics::BpMat4x4ToMat4x4(const BpMat4x4& input, Mat4x4* output)
{
	output->_11 = input.col0.x;
	output->_12 = input.col0.y;
	output->_13 = input.col0.z;
	output->_14 = input.col0.w;

	output->_21 = input.col1.x;
	output->_22 = input.col1.y;
	output->_23 = input.col1.z;
	output->_24 = input.col1.w;
	
	output->_31 = input.col2.x;
	output->_32 = input.col2.y;
	output->_33 = input.col2.z;
	output->_34 = input.col2.w;
	
	output->_41 = input.col3.x;
	output->_42 = input.col3.y;
	output->_43 = input.col3.z;
	output->_44 = input.col3.w;
}

void BombastPhysics::BpVec3ToVec3(const BpVec3& input, Vec3* output)
{
	output->x = input.x;
	output->y = input.y;
	output->z = input.z;
}

//Takes in BombastPhysics library shape and converts to BombastEngine base debug shape
IDebugPhysicsShape* BombastPhysics::ConvertPhysicsDebugShape(BpDebugShape* shape)
{
	Vec3 pos, color;
	BpVec3ToVec3(shape->position, &pos);
	BpVec3ToVec3(shape->color, &color);

	switch (shape->shapeType) {
		case DebugShapeType::SPHERE:
		{
			BpDebugSphere* sphere = static_cast<BpDebugSphere*>(shape);
			return BE_NEW BombastDebugPhysicsSphere(pos, color, sphere->radius);
		}
		case DebugShapeType::BOX:
		{
			BpDebugBox* box = static_cast<BpDebugBox*>(shape);
			Vec3 extent;
			BpVec3ToVec3(box->extent, &extent);
			return BE_NEW BombastDebugPhysicsBox(pos, color, extent);
		}
	}

	return nullptr;
}

IDebugPhysicsRenderBuffer* BombastPhysics::VGetDebugRenderBuffer()
{
	BombastPhysicsDebugRenderBuffer* newBuffer =  BE_NEW BombastPhysicsDebugRenderBuffer;
	
	std::vector<BpDebugShape*> shapes = m_pScene->GetRenderBuffer().m_debugShapes;
	for (std::vector<BpDebugShape*>::iterator it = shapes.begin(); it != shapes.end(); it++)
	{
		IDebugPhysicsShape* newShape = ConvertPhysicsDebugShape(*it);
		if (newShape == nullptr) 
		{
			BE_ERROR("Failed to Convert Debug Physics Shape. Skipping");
		}
		else 
		{
			newBuffer->m_shapes.push_back(newShape);
		}
	}

	return newBuffer;
}

BombastPhysicsDebugRenderBuffer::~BombastPhysicsDebugRenderBuffer()
{
	for (std::vector<IDebugPhysicsShape*>::iterator it = m_shapes.begin(); it != m_shapes.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	m_shapes.clear();
}

Mat4x4 BombastDebugPhysicsSphere::VGetTransform()
{
	Mat4x4 result = Mat4x4::g_Identity;
	result.SetPosition(m_position);
	result.SetScale(Vec3(m_radius, m_radius, m_radius));
	return result;
}

Mat4x4 BombastDebugPhysicsBox::VGetTransform()
{
	Mat4x4 result = Mat4x4::g_Identity;
	result.SetPosition(m_position);
	result.SetScale(m_extent);
	return result;
}