#include "PhysicsComponent.h"
#include "TransformComponent.h"

const float DEFAULT_MAX_VELOCITY = 7.5f;
const float DEFAULT_MAX_ANGULAR_VELOCITY = 7.5f;

const char* PhysicsComponent::g_Name = "PhysicsComponent";

PhysicsComponent::PhysicsComponent()
{
	m_location = Vec3(0, 0, 0);
	m_orientation = Vec3(0, 0, 0);
	m_scale = Vec3(1, 1, 1);

	m_acceleration = 0;
	m_angularAcceleration = 0;
	m_maxVelocity = DEFAULT_MAX_VELOCITY;
	m_maxAngularVelocity = DEFAULT_MAX_ANGULAR_VELOCITY;
}

PhysicsComponent::~PhysicsComponent()
{
	m_pGamePhysics->VRemoveActor(m_pOwner->GetId());
}

bool PhysicsComponent::VInitialize(rapidxml::xml_node<>* pData)
{
	m_pGamePhysics = g_pApp->m_pGame->VGetGamePhysics();
	if (!m_pGamePhysics)
	{
		return false;
	}

	return true;
}

void PhysicsComponent::VPostInit()
{
	if (m_pOwner)
	{
		BE_ERROR("Shape not supported" + m_shape);
	}
}

void PhysicsComponent::VUpdate(int deltaMs)
{
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name);

	if (!pTransformComponent)
	{
		BE_ERROR("No Transform Component!");
		return;
	}

	Mat4x4 transform = pTransformComponent->GetTransform();

	if (m_acceleration != 0)
	{
		float frameAcceleration = m_acceleration / 1000.0f * (float)deltaMs;

		Vec3 velocity(m_pGamePhysics->VGetVelocity(m_pOwner->GetId()));
		float magnitude = velocity.Length();

		Vec3 direction(transform.GetDirection());
		m_pGamePhysics->VApplyForce(direction, magnitude, m_pOwner->GetId());
	}

	if (m_angularAcceleration != 0)
	{
		float frameAngularAcceleration = m_angularAcceleration / 1000.0f * (float)deltaMs;
		m_pGamePhysics->VApplyTorque(transform.GetUp(), frameAngularAcceleration, m_pOwner->GetId());
	}
}

void PhysicsComponent::BuildTransform(rapidxml::xml_node<>* pTransformElement)
{
	BE_ASSERT(pTransformElement);

	rapidxml::xml_node<>* pPosition = pTransformElement->first_node("Position");
	if (pPosition)
	{
		m_location.x = (float)atof(pPosition->first_attribute("x")->value());
		m_location.y = (float)atof(pPosition->first_attribute("y")->value());
		m_location.z = (float)atof(pPosition->first_attribute("z")->value());
	}

	rapidxml::xml_node<>* pOrientation = pTransformElement->first_node("Orientation");
	if (pPosition)
	{
		m_orientation.x = (float)atof(pOrientation->first_attribute("x")->value());
		m_orientation.y = (float)atof(pOrientation->first_attribute("y")->value());
		m_orientation.z = (float)atof(pOrientation->first_attribute("z")->value());
	}

	rapidxml::xml_node<>* pScale = pTransformElement->first_node("Scale");
	if (pPosition)
	{
		m_scale.x = (float)atof(pScale->first_attribute("x")->value());
		m_scale.y = (float)atof(pScale->first_attribute("y")->value());
		m_scale.z = (float)atof(pScale->first_attribute("z")->value());
	}
}

void PhysicsComponent::ApplyForce(const Vec3& direction, float newtons)
{
	m_pGamePhysics->VApplyForce(direction, newtons, m_pOwner->GetId());
}

void PhysicsComponent::ApplyTorque(const Vec3& direction, float newtons)
{
	m_pGamePhysics->VApplyTorque(direction, newtons, m_pOwner->GetId());
}

bool PhysicsComponent::KMove(const Mat4x4& transform)
{
	return m_pGamePhysics->VMove(transform, m_pOwner->GetId());
}

void PhysicsComponent::ApplyAcceleration(float acceleration)
{
	m_acceleration = acceleration;
}

void PhysicsComponent::RemoveAcceleration()
{
	m_acceleration = 0;
}

void PhysicsComponent::ApplyAngularAcceleration(float acceleration)
{
	m_angularAcceleration = acceleration;
}

void PhysicsComponent::RemoveAngularAcceleration()
{
	m_angularAcceleration = 0;
}

Vec3 PhysicsComponent::GetVelocity()
{
	return m_pGamePhysics->VGetVelocity(m_pOwner->GetId());
}

void PhysicsComponent::SetVelocity(const Vec3& velocity)
{
	m_pGamePhysics->VSetVelocity(m_pOwner->GetId(), velocity);
}

void PhysicsComponent::SetPosition(float x, float y, float z)
{
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name);

	if (pTransformComponent)
	{
		Mat4x4 transform = pTransformComponent->GetTransform();
		Vec3 position = Vec3(x, y, z);
		transform.SetPosition(position);
		
		KMove(transform);
	}
	else
	{
		BE_ERROR("Calling SetPosition on PhysicsComponent on an actor without TransformComponent on")
	}
}

void PhysicsComponent::Stop()
{
	m_pGamePhysics->VStopActor(m_pOwner->GetId());
}