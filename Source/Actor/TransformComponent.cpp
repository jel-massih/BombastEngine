#include "Actor.h"
#include "TransformComponent.h"
#include "../BombastEngineStd.h"
#include "../Bombast/BombastApp.h"

const char* TransformComponent::g_Name = "TransformComponent";

bool TransformComponent::VInitialize(rapidxml::xml_node<>* pData)
{
	BE_ASSERT(pData);

	Vec3 yawPitchRoll = m_transform.GetYawPitchRoll();
	yawPitchRoll.x = XMConvertToDegrees(yawPitchRoll.x);
	yawPitchRoll.y = XMConvertToDegrees(yawPitchRoll.y);
	yawPitchRoll.z = XMConvertToDegrees(yawPitchRoll.z);

	Vec3 position = m_transform.GetPosition();

	rapidxml::xml_node<>* pPositionElement = pData->first_node("Position");
	if (pPositionElement)
	{
		double x = 0;
		double y = 0;
		double z = 0;
		x = atof(pPositionElement->first_attribute("x")->value());
		y = atof(pPositionElement->first_attribute("y")->value());
		z = atof(pPositionElement->first_attribute("z")->value());
		position = Vec3(x, y, z);
	}

	rapidxml::xml_node<>* pRelativePositionElement = pData->first_node("RelativePosition");
	if (pRelativePositionElement)
	{
		double x = 0;
		double y = 0;
		double z = 0;
		x = atof(pRelativePositionElement->first_attribute("x")->value()) * g_pApp->m_options.m_screenSize.x;
		y = atof(pRelativePositionElement->first_attribute("y")->value()) * g_pApp->m_options.m_screenSize.y;
		z = atof(pRelativePositionElement->first_attribute("z")->value());
		position = Vec3(x, y, z);
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
		yawPitchRoll = Vec3(yaw, pitch, roll);
	}

	Mat4x4 translation;
	translation.BuildTranslation(position);

	Mat4x4 rotation;
	rotation.BuildYawPitchRoll(XMConvertToRadians(yawPitchRoll.x), XMConvertToRadians(yawPitchRoll.y), XMConvertToRadians(yawPitchRoll.z));

	m_transform = rotation * translation;

	return true;
}

//@TODO: Check to make sure this works and outDoc isnt deallocated, breaking returned element
rapidxml::xml_node<>* TransformComponent::VGenerateXml()
{
	rapidxml::xml_document<> outDoc;
	rapidxml::xml_node<>* pBaseElement = outDoc.allocate_node(rapidxml::node_element, VGetName());

	rapidxml::xml_node<>* pPosition = outDoc.allocate_node(rapidxml::node_element, "Position");
	Vec3 pos(m_transform.GetPosition());
	pPosition->append_attribute(outDoc.allocate_attribute("x", ToStr(pos.x).c_str()));
	pPosition->append_attribute(outDoc.allocate_attribute("y", ToStr(pos.y).c_str()));
	pPosition->append_attribute(outDoc.allocate_attribute("z", ToStr(pos.z).c_str()));
	pBaseElement->append_node(pPosition);

	rapidxml::xml_node<>* pDirection = outDoc.allocate_node(rapidxml::node_element, "YawPitchRoll");
	Vec3 orient(m_transform.GetYawPitchRoll());
	orient.x = XMConvertToDegrees(orient.x);
	orient.y = XMConvertToDegrees(orient.y);
	orient.z = XMConvertToDegrees(orient.z);
	pDirection->append_attribute(outDoc.allocate_attribute("x", ToStr(orient.x).c_str()));
	pDirection->append_attribute(outDoc.allocate_attribute("y", ToStr(orient.y).c_str()));
	pDirection->append_attribute(outDoc.allocate_attribute("z", ToStr(orient.z).c_str()));
	pBaseElement->append_node(pDirection);
	
	return pBaseElement;
}