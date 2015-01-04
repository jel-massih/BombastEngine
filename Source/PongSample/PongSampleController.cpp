#include "PongSampleController.h"
#include "PongSampleEvents.h"
#include "../Graphics3D/SceneNode.h"

const float ACTOR_ACCELERATION = 1.0f;
const float ACTOR_ANGULAR_ACCELERATION = 22.0f;

PongSampleController::PongSampleController(SceneNode* object)
	:m_pObject(object)
{
	memset(m_bKey, 0x00, sizeof(m_bKey));
}

bool PongSampleController::VOnMouseDown(const Point& mousePos, const int radius, const std::string& buttonName)
{
	if (buttonName != "MouseLeft")
	{
		return false;
	}

	ActorId actorId = m_pObject->VGet()->GetActorId();
	BE_ASSERT_MESSAGE(actorId != INVALID_ACTOR_ID, "The Pong Sample Controller isnt attached to a valid actor");
	std::shared_ptr<EvtData_Fire_Weapon> pFireEvent(BE_NEW EvtData_Fire_Weapon(actorId));
	IEventManager::Get()->VQueueEvent(pFireEvent);
	return true;
}

void PongSampleController::OnUpdate(DWORD const deltaMs)
{

}

bool PongSampleController::VOnKeyDown(const BYTE c)
{
	m_bKey[c] = true;

	if (c == 'W' || c == 'S')
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_StartForward> pEvent(BE_NEW EvtData_StartForward(actorId, (c == 'W' ? ACTOR_ACCELERATION : (-ACTOR_ACCELERATION))));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	if (c == 'A' || c == 'D')
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_StartLeft> pEvent(BE_NEW EvtData_StartLeft(actorId, (c == 'A' ? ACTOR_ACCELERATION : (-ACTOR_ACCELERATION))));
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
		std::shared_ptr<EvtData_StartTurnLeft> pEvent(BE_NEW EvtData_StartTurnLeft(actorId, (c == VK_LEFT ? ACTOR_ACCELERATION : (-ACTOR_ACCELERATION))));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	if (c == VK_UP || c == VK_DOWN)
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_StartTurnUp> pEvent(BE_NEW EvtData_StartTurnUp(actorId, (c == VK_UP ? ACTOR_ACCELERATION : (-ACTOR_ACCELERATION))));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	return true;
}

bool PongSampleController::VOnKeyUp(const BYTE c)
{
	m_bKey[c] = false;

	if (c == 'W' || c == 'S')
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_EndForward> pEvent(BE_NEW EvtData_EndForward(actorId));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	if (c == 'A' || c == 'D')
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_EndLeft> pEvent(BE_NEW EvtData_EndLeft(actorId));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	if (c == 'Z' || c == 'X')
	{
		const ActorId actorId = m_pObject->VGet()->GetActorId();
		std::shared_ptr<EvtData_EndUp> pEvent(BE_NEW EvtData_EndUp(actorId));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	return true;
}