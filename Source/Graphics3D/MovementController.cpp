#include "MovementController.h"
#include <iostream>

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

MovementController::MovementController(SceneNode* pObject, float initialYaw, float initialPitch, bool rotateWhenLButtonDown)
	:m_pObject(pObject)
{
	pObject->VGet()->Transform(&m_matToWorld, &m_matFromWorld);

	m_targetYaw = m_yaw = XMConvertToDegrees(-initialYaw);
	m_targetPitch = m_pitch = XMConvertToDegrees(initialPitch);

	m_maxSpeed = 30.0f;
	m_currentSpeed = 0.0f;

	Vec3 pos = m_matToWorld.GetPosition();
	m_matPosition.BuildTranslation(pos);

	POINT ptCursor;
	GetCursorPos(&ptCursor);
	m_lastMousePos = ptCursor;

	memset(m_bKey, 0x00, sizeof(m_bKey));
}

void MovementController::SetObject(SceneNode* pNewObject)
{
}

void MovementController::OnUpdate(DWORD const deltaMs)
{
	bool bTranslating = false;
	Vec4 atWorld(0, 0, 0, 0);
	Vec4 rightWorld(0, 0, 0, 0);
	Vec4 upWorld(0, 0, 0, 0);

	if (m_bKey['W'] || m_bKey['S'])
	{
		Vec4 at = g_Forward4;
		if (m_bKey['S'])
		{
			at *= -1;
		}

		//Gets "Look At" Vector
		atWorld = m_matToWorld.Xform(at);
		bTranslating = true;	
	}

	if (m_bKey['A'] || m_bKey['D'])
	{
		Vec4 right = g_Right4;
		if (m_bKey['A'])
		{
			right *= -1;
		}

		//Gets "Right" vector
		rightWorld = m_matToWorld.Xform(right);
		bTranslating = true;
	}

	if (m_bKey['C'] || m_bKey['X'])
	{
		Vec4 up = g_Up4;
		if (!m_bKey['C'])
		{
			up *= -1;
		}

		upWorld = m_matToWorld.Xform(up);
		bTranslating = true;
	}

	m_yaw += (m_targetYaw - m_yaw) * (0.35f);
	m_targetPitch = MAX(-90, MIN(90, m_targetPitch));
	m_pitch += (m_targetPitch - m_pitch) * (0.35f);

	Mat4x4 matRot;
	matRot.BuildYawPitchRoll(XMConvertToRadians(-m_yaw), XMConvertToRadians(m_pitch), 0.0f);

	m_matToWorld = matRot * m_matPosition;
	m_matFromWorld = m_matToWorld.Inverse();
	m_pObject->VSetTransform(&m_matToWorld, &m_matFromWorld);

	if (bTranslating)
	{
		float elapsedTime = (float)deltaMs / 1000.0f;

		Vec3 dir = atWorld + rightWorld + upWorld;
		dir.Normalize();

		float numSeconds = 5.f;
		m_currentSpeed += m_maxSpeed * ((elapsedTime*elapsedTime) / numSeconds);
		if (m_currentSpeed > m_maxSpeed)
		{
			m_currentSpeed = m_maxSpeed;
		}

		dir *= m_currentSpeed;

		Vec3 pos = m_matPosition.GetDirection() + dir;
		m_matPosition.SetPosition(pos);
		m_matToWorld.SetPosition(pos);

		m_matFromWorld = m_matToWorld.Inverse();
		m_pObject->VSetTransform(&m_matToWorld, &m_matFromWorld);
	}
	else
	{
		m_currentSpeed = 0.0f;
	}
}

bool MovementController::VOnMouseMove(const Point &pos, const int radius)
{
	if (m_lastMousePos != pos)
	{
		//m_targetYaw += m_lastMousePos.x - pos.x;
		//m_targetPitch += pos.y - m_lastMousePos.y;

		m_lastMousePos = pos;
	}
	return true;
}

bool MovementController::VOnMouseDown(const Point &pos, const int radius, const std::string &buttonName)
{
	return false;
}

bool MovementController::VOnMouseUp(const Point &pos, const int radius, const std::string &buttonName)
{
	return false;
}