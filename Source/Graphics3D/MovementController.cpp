#include "MovementController.h"

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
	Mat4x4 rotationMatrix = Mat4x4::g_Identity;
	
	rotationMatrix.BuildYawPitchRoll(m_yaw, m_pitch, 0.0f);

	m_matToWorld = m_matPosition * rotationMatrix;
	m_matFromWorld = m_matToWorld.Inverse();
	m_pObject->VSetTransform(&m_matToWorld, &m_matFromWorld);
}

bool MovementController::VOnMouseMove(const Point &pos, const int radius)
{
	if (m_lastMousePos != pos)
	{
		m_targetYaw += m_lastMousePos.x - pos.x;
		m_targetPitch += pos.y - m_lastMousePos.y;

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