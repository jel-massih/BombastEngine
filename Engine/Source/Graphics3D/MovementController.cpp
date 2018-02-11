#include "MovementController.h"

#include "../InputCore/InputCore.h"

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

MovementController::MovementController(SceneNode* pObject, float initialYaw, float initialPitch)
	: m_pObject(pObject)
{
	pObject->VGet()->Transform(&m_matToWorld, &m_matFromWorld);

	m_targetYaw = m_yaw = XMConvertToDegrees(-initialYaw);
	m_targetPitch = m_pitch = XMConvertToDegrees(initialPitch);

	m_maxSpeed = 10.0f;
	m_forwardSpeed = m_backwardSpeed = m_leftSpeed = m_rightSpeed = m_upSpeed = m_downSpeed = 0.0f;

	Vec3 pos = m_matToWorld.GetPosition();
	m_matPosition.BuildTranslation(pos);

	POINT ptCursor;
	GetCursorPos(&ptCursor);

	m_accelRate = 5;
	m_deccelRate = 1;
	m_turnRate = 100;
}

void MovementController::SetObject(SceneNode* pNewObject)
{
}

void MovementController::OnUpdate(const float deltaMs)
{
	float deltaSeconds = (deltaMs / 1000.f);

	Mat4x4 rotationMatrix = Mat4x4::g_Identity;
	Vec3 moveDirection = Vec3(0, 0, 0);

	const InputCore* pInputCore = g_pApp->GetInputCore();

	if (pInputCore->IsKeyHeld((BYTE)VK_LEFT) || pInputCore->IsKeyHeld((BYTE)VK_RIGHT))
	{
		int dir = pInputCore->IsKeyHeld((BYTE)VK_RIGHT) ? 1 : -1;
		m_targetPitch += m_turnRate * dir * deltaSeconds;
	}

	if (pInputCore->IsKeyHeld((BYTE)VK_UP) || pInputCore->IsKeyHeld((BYTE)VK_DOWN))
	{
		int dir = pInputCore->IsKeyHeld((BYTE)VK_DOWN) ? 1 : -1;
		m_targetYaw += m_turnRate * dir * deltaSeconds;
	}

	if (m_targetPitch >= 360)
	{
		m_targetPitch = 0;
	}

	if (m_targetYaw >= 90)
	{
		m_targetYaw = 90;
	}

	if (m_targetYaw <= -90)
	{
		m_targetYaw = -90;
	}

	m_pitch = XMConvertToRadians(m_targetPitch);
	m_yaw = XMConvertToRadians(m_targetYaw);

	Vec3 pos = m_matPosition.GetPosition();

	if (pInputCore->IsKeyHeld('W'))
	{
		m_forwardSpeed += deltaSeconds * m_accelRate;
		m_forwardSpeed = MIN(m_forwardSpeed, m_maxSpeed * deltaSeconds);
	}
	else if (m_forwardSpeed > 0.0f)
	{
		m_forwardSpeed -= deltaSeconds * m_deccelRate;
		m_forwardSpeed = MAX(m_forwardSpeed, 0.0f);
	}

	if (pInputCore->IsKeyHeld('S'))
	{
		m_backwardSpeed += deltaSeconds * m_accelRate;
		m_backwardSpeed = MIN(m_backwardSpeed, m_maxSpeed * deltaSeconds);
	}
	else if (m_backwardSpeed > 0.0f)
	{
		m_backwardSpeed -= deltaSeconds * m_deccelRate;
		m_backwardSpeed = MAX(m_backwardSpeed, 0.0f);
	}

	if (pInputCore->IsKeyHeld('A'))
	{
		m_leftSpeed += deltaSeconds * m_accelRate;
		m_leftSpeed = MIN(m_leftSpeed, m_maxSpeed * deltaSeconds);
	}
	else if (m_leftSpeed > 0.0f)
	{
		m_leftSpeed -= deltaSeconds * m_deccelRate;
		m_leftSpeed = MAX(m_leftSpeed, 0.0f);
	}

	if(pInputCore->IsKeyHeld('D'))
	{
		m_rightSpeed += deltaSeconds * m_accelRate;
		m_rightSpeed = MIN(m_rightSpeed, m_maxSpeed * deltaSeconds);
	}
	else if (m_rightSpeed > 0.0f)
	{
		m_rightSpeed -= deltaSeconds * m_deccelRate;
		m_rightSpeed = MAX(m_rightSpeed, 0.0f);
	}

	rotationMatrix.BuildYawPitchRoll(m_yaw, m_pitch, 0.0f);

	Vec3 rot = rotationMatrix.GetYawPitchRoll();

	moveDirection.x += sinf(rot.x) * (m_forwardSpeed - m_backwardSpeed);
	moveDirection.z += cosf(rot.x) * (m_forwardSpeed - m_backwardSpeed);
	moveDirection.y -= sinf(rot.y) * (m_forwardSpeed - m_backwardSpeed);

	moveDirection.x += cosf(rot.x) * (m_rightSpeed - m_leftSpeed);
	moveDirection.z -= sinf(rot.x) * (m_rightSpeed - m_leftSpeed);

	m_matPosition.SetPosition(pos + moveDirection);

	m_matToWorld = rotationMatrix * m_matPosition;
	m_matFromWorld = m_matToWorld.Inverse();
	m_pObject->VSetTransform(&m_matToWorld, &m_matFromWorld);
}

void MovementController::VProcessInput()
{
	float mouseAxisX, mouseAxisY;
	g_pApp->GetInputCore()->GetMouseAxis(mouseAxisX, mouseAxisY);

	if (mouseAxisX > 0.f || mouseAxisX < 0.f)
	{
		m_targetPitch += mouseAxisX * 20.f;
		m_targetYaw += mouseAxisY * 20.f;
	}
}