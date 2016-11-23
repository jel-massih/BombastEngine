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

	rapidxml::xml_node<>* pScaleElement = pData->first_node("Scale");
	if (pScaleElement)
	{
		double x = 0;
		double y = 0;
		double z = 0;
		x = atof(pScaleElement->first_attribute("x")->value());
		y = atof(pScaleElement->first_attribute("y")->value());
		z = atof(pScaleElement->first_attribute("z")->value());
		m_scale = Vec3(x, y, z);
	}

	UpdateTransform();

	return true;
}

//@TODO: Check to make sure this works and outDoc isnt deallocated, breaking returned element
rapidxml::xml_node<>* TransformComponent::VGenerateXml(rapidxml::xml_document<> &outDoc)
{
	rapidxml::xml_node<>* pBaseElement = outDoc.allocate_node(rapidxml::node_element, VGetName());

	rapidxml::xml_node<>* pPosition = outDoc.allocate_node(rapidxml::node_element, "Position");
	char *pos_x = outDoc.allocate_string(ToStr(m_position.x).c_str());
	pPosition->append_attribute(outDoc.allocate_attribute("x", pos_x));
	char *pos_y = outDoc.allocate_string(ToStr(m_position.y).c_str());
	pPosition->append_attribute(outDoc.allocate_attribute("y", pos_y));
	char *pos_z = outDoc.allocate_string(ToStr(m_position.z).c_str());
	pPosition->append_attribute(outDoc.allocate_attribute("z", pos_z));
	pBaseElement->append_node(pPosition);

	rapidxml::xml_node<>* pDirection = outDoc.allocate_node(rapidxml::node_element, "YawPitchRoll");
	char *dir_x = outDoc.allocate_string(ToStr(m_rotation.x).c_str());
	pDirection->append_attribute(outDoc.allocate_attribute("x", dir_x));
	char *dir_y = outDoc.allocate_string(ToStr(m_rotation.y).c_str());
	pDirection->append_attribute(outDoc.allocate_attribute("y", dir_y));
	char *dir_z = outDoc.allocate_string(ToStr(m_rotation.z).c_str());
	pDirection->append_attribute(outDoc.allocate_attribute("z", dir_z));
	pBaseElement->append_node(pDirection);

	rapidxml::xml_node<>* pScale = outDoc.allocate_node(rapidxml::node_element, "Scale");
	char *scale_x = outDoc.allocate_string(ToStr(m_scale.x).c_str());
	pScale->append_attribute(outDoc.allocate_attribute("x", scale_x));
	char *scale_y = outDoc.allocate_string(ToStr(m_scale.y).c_str());
	pScale->append_attribute(outDoc.allocate_attribute("y", scale_y));
	char *scale_z = outDoc.allocate_string(ToStr(m_scale.z).c_str());
	pScale->append_attribute(outDoc.allocate_attribute("z", scale_z));
	pBaseElement->append_node(pScale);
	
	return pBaseElement;
}

void TransformComponent::UpdateTransform()
{
	Mat4x4 translation;
	translation.BuildTranslation(m_position);

	Mat4x4 rotation;
	rotation.BuildYawPitchRoll(XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.z));

	Mat4x4 scale;
	scale.BuildScale(m_scale);

	m_transform = scale * rotation * translation;
}

void TransformComponent::MoveForward(float magnitude)
{
	float radians;

	radians = XMConvertToRadians(m_rotation.y);

	m_position.z += cosf(radians) * magnitude;
	m_position.y += sinf(radians) * magnitude;

	UpdateTransform();
}

void TransformComponent::MoveUp(float magnitude)
{
	m_position.y += magnitude;
	UpdateTransform();
}

void TransformComponent::MoveLeft(float magnitude)
{
	float radians;

	radians = XMConvertToRadians(m_rotation.x);

	m_position.x += cosf(radians) * -magnitude;
	m_position.y += sinf(radians) * -magnitude;

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