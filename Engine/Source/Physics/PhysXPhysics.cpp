#include "PhysXPhysics.h"
#include "../Actor/TransformComponent.h"
#include "../Resources/XmlResource.h"
#include "../Events/Events.h"

#define ENABLE_PHYSX_PVD true

const PxReal PhysXPhysics::Timestep = 1.00f / 60.0f;

PhysXPhysics::PhysXPhysics()
{
	m_pFoundation = nullptr;
	m_pPhysicsSdk = nullptr;
	m_pDispatcher = nullptr;
	m_pScene = nullptr;
	m_pConnection = nullptr;
}

PhysXPhysics::~PhysXPhysics()
{
	m_pScene->release();
	m_pDispatcher->release();
	if (m_pConnection != nullptr)
	{
		m_pConnection->release();
	}
	m_pPhysicsSdk->release();
	m_pFoundation->release();
}

bool PhysXPhysics::VInitialize()
{
	VLoadPhysicsConfigXml();

	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocatorCallback, m_errorCallback);
	m_pPhysicsSdk = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true);

	if (!m_pPhysicsSdk)
	{
		BE_ERROR("Error Creating PhysX device.");
		return false;
	}

	PxSceneDesc sceneDesc(m_pPhysicsSdk->getTolerancesScale());

	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); //Set Gravity
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
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
	m_pConnection = PxVisualDebuggerExt::createConnection(m_pPhysicsSdk->getPvdConnectionManager(), pvd_host_ip, port, timeout, connectionFlags);
}

void PhysXPhysics::AddShape(Actor* pActor, PxGeometry* geometry, float density, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping)
{
	BE_ASSERT(pActor);
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
		//Doesnt work without transform
		return;
	}

	PhysicsMaterialData material(LookupMaterialData(physicsMaterial));
	PxMaterial* mat = m_pPhysicsSdk->createMaterial(material.m_friction, material.m_friction, material.m_restitution);

	PxMat44 pxMat;
	Mat4x4ToPxMatrix(transform, &pxMat);
	PxTransform t(pxMat);
	PxRigidDynamic* body = PxCreateDynamic(*m_pPhysicsSdk, t, *geometry, *mat, density);
	body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !gravityEnabled);
	PxRigidBodyExt::updateMassAndInertia(*body, density);
	body->setLinearDamping(linearDamping);
	body->setAngularDamping(angularDamping);
	m_pScene->addActor(*body);

	m_actorRigidBodyMap[actorId] = body;
	m_rigidBodyActorMap[body] = actorId;
}

void PhysXPhysics::RemovePhysicsObject(PxRigidBody* body)
{
	m_pScene->removeActor(*body);
}

PhysicsMaterialData PhysXPhysics::LookupMaterialData(const std::string& materialString)
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

float PhysXPhysics::LookupDensity(const std::string& densityString)
{
	float density = 1.0f;
	auto it = m_densityTable.find(densityString);
	if (it != m_densityTable.end())
	{
		density = it->second;
	}

	return density;
}

PxRigidBody* PhysXPhysics::FindRigidBody(ActorId const actorId)
{
	ActorIdToPysXRigidBodyTable::const_iterator found = m_actorRigidBodyMap.find(actorId);
	if (found != m_actorRigidBodyMap.end())
	{
		return found->second;
	}

	return NULL;
}

ActorId PhysXPhysics::FindActorId(PxRigidBody const * const body)
{
	PysXRigidBodyToActorIdTable::const_iterator found = m_rigidBodyActorMap.find(body);
	if (found != m_rigidBodyActorMap.end())
	{
		return found->second;
	}

	return ActorId();
}

void PhysXPhysics::Mat4x4ToPxMatrix(const Mat4x4& input, PxMat44* output)
{
	output->column0.x = input._11;
	output->column0.y = input._12;
	output->column0.z = input._13;
	output->column0.w = input._14;
							 
	output->column1.x = input._21;
	output->column1.y = input._22;
	output->column1.z = input._23;
	output->column1.w = input._24;
							 
	output->column2.x = input._31;
	output->column2.y = input._32;
	output->column2.z = input._33;
	output->column2.w = input._34;
							 
	output->column3.x = input._41;
	output->column3.y = input._42;
	output->column3.z = input._43;
	output->column3.w = input._44;
}

void PhysXPhysics::PxMatrixToMat4x4(const PxMat44& input, Mat4x4* output)
{
	output->_11 = input.column0.x;
	output->_12 = input.column0.y;
	output->_13 = input.column0.z;
	output->_14 = input.column0.w;

	output->_21 = input.column1.x;
	output->_22 = input.column1.y;
	output->_23 = input.column1.z;
	output->_24 = input.column1.w;

	output->_31 = input.column2.x;
	output->_32 = input.column2.y;
	output->_33 = input.column2.z;
	output->_34 = input.column2.w;

	output->_41 = input.column3.x;
	output->_42 = input.column3.y;
	output->_43 = input.column3.z;
	output->_44 = input.column3.w;
}

void PhysXPhysics::PxVecToVec3(const PxVec3& input, Vec3* output)
{
	output->x = input.x;
	output->y = input.y;
	output->z = input.z;
}

void PhysXPhysics::VLoadPhysicsConfigXml()
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
		m_densityTable.insert(std::make_pair(pNode->name(),(float)atof(pNode->value())));
	}
}

void PhysXPhysics::VOnUpdate(float const deltaMs)
{
	m_pScene->simulate(PhysXPhysics::Timestep);
	m_pScene->fetchResults(true);
}

void PhysXPhysics::VSyncVisibleScene()
{
	for (ActorIdToPysXRigidBodyTable::const_iterator it = m_actorRigidBodyMap.begin(); it != m_actorRigidBodyMap.end(); it++)
	{
		ActorId const id = it->first;

		PxTransform pxLoc = it->second->getGlobalPose();
		Mat4x4 loc;
		PxMatrixToMat4x4(PxMat44(pxLoc), &loc);
		
		Actor* pActor = g_pApp->m_pGame->VGetActor(id);
		if (pActor)
		{
			TransformComponent* pTransformComponent = pActor->GetComponent<TransformComponent>(TransformComponent::g_Name);

			if (pTransformComponent)
			{
				if (pTransformComponent->GetTransform() != loc)
				{
					pTransformComponent->SetTransform(loc);
					EventDataPtr pEvent(BE_NEW EvtData_Move_Actor(id, loc));
					IEventManager::Get()->VQueueEvent(pEvent);
				}
			}
		}
	}
}

void PhysXPhysics::VAddSphere(float radius, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform)
{
	float density = LookupDensity(densityStr);

	AddShape(gameActor, &PxSphereGeometry(radius), density, physicsMaterial, gravityEnabled, linearDamping, angularDamping);
}

void PhysXPhysics::VAddBox(Vec3 scale, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform)
{
	float density = LookupDensity(densityStr);

	AddShape(gameActor, &PxBoxGeometry(scale.x, scale.y, scale.z), density, physicsMaterial, gravityEnabled, linearDamping, angularDamping);
}

void PhysXPhysics::VAddCapsule(float radius, float halfHeight, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping, Mat4x4 relativeTransform)
{
	float density = LookupDensity(densityStr);

	AddShape(gameActor, &PxCapsuleGeometry(radius, halfHeight), density, physicsMaterial, gravityEnabled, linearDamping, angularDamping);
}

void PhysXPhysics::VRemoveActor(ActorId id)
{
	if (PxRigidBody* const body = FindRigidBody(id))
	{
		RemovePhysicsObject(body);
		m_actorRigidBodyMap.erase(id);
		m_rigidBodyActorMap.erase(body);
		body->release();
	}
}

void PhysXPhysics::VSetDebugVisualizationEnabled(bool enabled)
{
	throw "Function not yet implemented.";
}

void PhysXPhysics::VRenderDiagnostics()
{
	throw "Function not yet implemented.";
}

void PhysXPhysics::VApplyForce(const Vec3 &dir, float newtons, ActorId actorId)
{
	if (PxRigidBody* const body = FindRigidBody(actorId))
	{
		PxVec3 const force(dir.x * newtons, dir.y * newtons, dir.z * newtons);
		body->addForce(force);
	}
}

void PhysXPhysics::VApplyTorque(const Vec3 &dir, float newtons, ActorId actorId)
{
	if (PxRigidBody* const body = FindRigidBody(actorId))
	{
		PxVec3 const force(dir.x * newtons, dir.y * newtons, dir.z * newtons);
		body->addTorque(force);
	}
}

bool PhysXPhysics::VMove(const Mat4x4 &mat, ActorId actorId)
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
	PxRigidBody* pBody = FindRigidBody(actorId);
	BE_ASSERT(pBody);
	if (!pBody)
	{
		return Vec3::g_InvalidVec3;
	}
	PxVec3 pxVec = pBody->getLinearVelocity();

	Vec3 velocity;
	PxVecToVec3(pxVec, &velocity);

	return velocity;
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

IDebugPhysicsRenderBuffer* PhysXPhysics::VGetDebugRenderBuffer()
{
	throw "Function not yet implemented.";
}