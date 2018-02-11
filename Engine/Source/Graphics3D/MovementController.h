#pragma once

class MovementController : public IInputHandler
{
public:
	MovementController(SceneNode* pObject, float initialYaw, float initialPitch, bool rotateWhenRButtonDown);
	void SetObject(SceneNode* pNewObject);

	void OnUpdate(const float deltaMs);

public:
	void VProcessInput();

	const Mat4x4 *GetToWorld() { return &m_matToWorld; }
	const Mat4x4 *GetFromWorld() { return &m_matFromWorld; }

protected:
	Mat4x4  m_matFromWorld;
	Mat4x4	m_matToWorld;
	Mat4x4  m_matPosition;

	float m_targetYaw;
	float m_targetPitch;
	float m_yaw;
	float m_pitch;

	float m_maxSpeed;
	float m_accelRate;
	float m_deccelRate;

	float m_forwardSpeed, m_backwardSpeed, m_leftSpeed, m_rightSpeed, m_upSpeed, m_downSpeed;

	float m_turnRate;

	bool m_bCameraTrackMouseOnRightHold;
	bool m_bCameraTrackMouse;

	SceneNode* m_pObject;
};