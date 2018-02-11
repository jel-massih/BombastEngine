#pragma once
#include "BombastEngineStd.h"
#include "Bombast/interfaces.h"

class SceneNode;

class GSPlayerController : public IInputHandler
{
public:
	GSPlayerController(SceneNode* object, float initialYaw, float initialPitch);
	void OnUpdate(const float deltaMs);

protected:
	Mat4x4  m_matFromWorld;
	Mat4x4	m_matToWorld;

	BYTE m_bKey[256];
	SceneNode* m_pObject;

	float m_targetYaw;
	float m_targetPitch;
	float m_yaw;
	float m_pitch;

	float m_turnRate;

public:
	void VProcessInput();
};