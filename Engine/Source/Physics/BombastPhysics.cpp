#include "BombastPhysics.h"
#include "../Actor/TransformComponent.h"
#include "../Resources/XmlResource.h"
#include "../Events/Events.h"

const float BombastPhysics::Timestep = 1.00f / 60.0f;

BombastPhysics::BombastPhysics()
{
}

BombastPhysics::~BombastPhysics()
{
}

bool BombastPhysics::VInitialize()
{
	VLoadPhysicsConfigXml();

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
	throw "Function not yet implemented.";
}

void BombastPhysics::VSyncVisibleScene()
{
	throw "Function not yet implemented.";
}

void BombastPhysics::VAddSphere(float radius, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping)
{
	throw "Function not yet implemented.";
}

void BombastPhysics::VRemoveActor(ActorId id)
{
	throw "Function not yet implemented.";
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