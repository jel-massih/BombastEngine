#include "GSPlayerController.h"
#include "../GameSampleEvents.h"
#include "Graphics3D/SceneNode.h"

const float ACTOR_ACCELERATION = 1.0f;
const float ACTOR_ANGULAR_ACCELERATION = 22.0f;

GSPlayerController::GSPlayerController(SceneNode* object, float initialYaw, float initialPitch)
	:m_pObject(object), m_turnRate(100)
{
	memset(m_bKey, 0x00, sizeof(m_bKey));

	m_targetYaw = m_yaw = XMConvertToDegrees(-initialYaw);
	m_targetPitch = m_pitch = XMConvertToDegrees(initialPitch);
}

bool GSPlayerController::VOnMouseDown(const Point& mousePos, const int radius, const std::string& buttonName)
{
	if (buttonName != "MouseLeft")
	{
		return false;
	}

	ActorId actorId = m_pObject->VGet()->GetActorId();
	BE_ASSERTf(actorId != INVALID_ACTOR_ID, "The Pong Sample Controller isnt attached to a valid actor");
	std::shared_ptr<EvtData_Fire_Weapon> pFireEvent(BE_NEW EvtData_Fire_Weapon(actorId));
	IEventManager::Get()->VQueueEvent(pFireEvent);
	return true;
}

bool GSPlayerController::VOnMouseMove(const Point& pos, const int radius)
{
	Point screenSize = g_pApp->GetScreenSize();
	const int mouseDeltaX = pos.GetX() - (screenSize.GetX() / 2);
	const int mouseDeltaY = pos.GetY() - (screenSize.GetY() / 2);
	POINT pt;
	pt.x = (screenSize.GetX() / 2);
	pt.y = (screenSize.GetY() / 2);
	ClientToScreen(g_pApp->GetHwnd(), &pt);
	SetCursorPos(pt.x, pt.y);

	float lookRightDeg = (float)(mouseDeltaX * 0.1);
	float lookUpDeg = (float)(mouseDeltaY * 0.1);

	static const float zenithMinDeclination = 1;
	static const float zenithMaxDeclination = 180 - 1;

	m_targetPitch += lookRightDeg;
	m_targetYaw += lookUpDeg;

	return true;
}

void GSPlayerController::OnUpdate(const float deltaMs)
{
	Mat4x4 rotationMatrix = Mat4x4::g_Identity;
	Mat4x4 positionMatrix = Mat4x4::g_Identity;

	if (m_targetPitch >= 360)
	{
		m_targetPitch = 0;
	}

	if (m_targetYaw >= 70)
	{
		m_targetYaw = 70;
	}

	if (m_targetYaw <= -50)
	{
		m_targetYaw = -50;
	}

	m_pitch = XMConvertToRadians(m_targetPitch);
	m_yaw = XMConvertToRadians(m_targetYaw);

	rotationMatrix.BuildYawPitchRoll(m_yaw, m_pitch, 0.0f);
	positionMatrix.BuildTranslation(m_pObject->VGet()->ToWorld().GetPosition());

	m_matToWorld = rotationMatrix * positionMatrix;
	m_matFromWorld = m_matToWorld.Inverse();

	m_pObject->VSetTransform(&m_matToWorld, &m_matFromWorld);
}

bool GSPlayerController::VOnKeyDown(const BYTE c)
{
	m_bKey[c] = true;

	if (c == 'W' || c == 'S')
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_StartForward> pEvent(BE_NEW EvtData_StartForward(actorId, (c == 'W' ? -ACTOR_ACCELERATION : ACTOR_ACCELERATION)));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	if (c == 'A' || c == 'D')
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_StartRight> pEvent(BE_NEW EvtData_StartRight(actorId, (c == 'A' ? -ACTOR_ACCELERATION : ACTOR_ACCELERATION)));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	if (c == VK_CONTROL || c == VK_MENU)
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_StartUp> pEvent(BE_NEW EvtData_StartUp(actorId, (c == 'Z' ? ACTOR_ACCELERATION : (-ACTOR_ACCELERATION))));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	if (c == VK_LEFT || c == VK_RIGHT)
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_StartTurnRight> pEvent(BE_NEW EvtData_StartTurnRight(actorId, (c == VK_LEFT ? -ACTOR_ACCELERATION * 3 : ACTOR_ACCELERATION * 3)));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	if (c == VK_UP || c == VK_DOWN)
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_StartTurnUp> pEvent(BE_NEW EvtData_StartTurnUp(actorId, (c == VK_UP ? -ACTOR_ACCELERATION * 3 : ACTOR_ACCELERATION * 3)));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	return true;
}

bool GSPlayerController::VOnKeyUp(const BYTE c)
{
	m_bKey[c] = false;

	if (c == 'W' || c == 'S')
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_EndForward> pEvent(BE_NEW EvtData_EndForward(actorId, c == 'S'));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	if (c == 'A' || c == 'D')
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_EndRight> pEvent(BE_NEW EvtData_EndRight(actorId, c == 'A'));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	if (c == 'Z' || c == 'X')
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_EndUp> pEvent(BE_NEW EvtData_EndUp(actorId, c == 'Z'));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	return true;
}