#include "Actor.h"
#include "TransformComponent.h"
#include "../BombastEngineStd.h"
#include "../Bombast/BombastApp.h"

const char* TransformComponent::g_Name = "TransformComponent";

bool TransformComponent::VInitialize(rapidxml::xml_node<>* pData)
{
	BE_ASSERT(pData);

	rapidxml::xml_node<>* pPositionElement = pData->first_node("Position");
	if (pPositionElement)
	{
		double x = 0;
		double y = 0;
		double z = 0;
		x = atof(pPositionElement->first_attribute("x")->value());
		y = atof(pPositionElement->first_attribute("y")->value());
		z = atof(pPositionElement->first_attribute("z")->value());
		m_position = Vec3(x, y, z);
	}

	rapidxml::xml_node<>* pOrientationElement = pData->first_node("YawPitchRoll");
	if (pOrientationElement)
	{
		double yaw = 0;
		double pitch = 0;
		double roll = 0;

		yaw = atof(pOrientationElement->first_attribute("x")->value());
		pitch = atof(pOrientationElement->first_attribute("y")->value());
		roll = atof(pOrientationElement->first_attribute("z")->value());
		m_rotation = Vec3(yaw, pitch, roll);
	}

	UpdateTransform();

	return true;
}

//@TODO: Check to make sure this works and outDoc isnt deallocated, breaking returned element
rapidxml::xml_node<>* TransformComponent::VGenerateXml()
{
	rapidxml::xml_document<> outDoc;
	rapidxml::xml_node<>* pBaseElement = outDoc.allocate_node(rapidxml::node_element, VGetName());

	rapidxml::xml_node<>* pPosition = outDoc.allocate_node(rapidxml::node_element, "Position");
	pPosition->append_attribute(outDoc.allocate_attribute("x", ToStr(m_position.x).c_str()));
	pPosition->append_attribute(outDoc.allocate_attribute("y", ToStr(m_position.y).c_str()));
	pPosition->append_attribute(outDoc.allocate_attribute("z", ToStr(m_position.z).c_str()));
	pBaseElement->append_node(pPosition);

	rapidxml::xml_node<>* pDirection = outDoc.allocate_node(rapidxml::node_element, "YawPitchRoll");
	pDirection->append_attribute(outDoc.allocate_attribute("x", ToStr(m_rotation.x).c_str()));
	pDirection->append_attribute(outDoc.allocate_attribute("y", ToStr(m_rotation.y).c_str()));
	pDirection->append_attribute(outDoc.allocate_attribute("z", ToStr(m_rotation.z).c_str()));
	pBaseElement->append_node(pDirection);
	
	return pBaseElement;
}

void TransformComponent::UpdateTransform()
{
	Mat4x4 translation;
	translation.BuildTranslation(m_position);

	Mat4x4 rotation;
	rotation.BuildYawPitchRoll(XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.z));

	m_transform = rotation * translation;
}

void TransformComponent::MoveForward(float magnitude)
{
	float radians;

	radians = XMConvertToRadians(m_rotation.y);

	m_position.x += sinf(radians) * magnitude;
	m_position.y += cosf(radians) * magnitude;

	UpdateTransform();
}

void TransformComponent::MoveUp(float magnitude)
{
	m_position.y += magnitude;
	UpdateTransform();
}

void TransformComponent::RotateRight(float magnitude)
{
	m_rotation.y += magnitude;

	if (m_rotation.y > 360.0f)
	{
		m_rotation.y -= 360.0f;
	}
	else if (m_rotation.y < 0.0f)
	{
		m_rotation.y += 360.0f;
	}

	UpdateTransform();
}

void TransformComponent::RotateUp(float magnitude)
{
	m_rotation.x += magnitude;

	if (m_rotation.x > 90.0f)
	{
		m_rotation.x = 90.0f;
	}
	else if (m_rotation.x < -90.0f)
	{
		m_rotation.x = -90.0f;
	}

	UpdateTransform();
}