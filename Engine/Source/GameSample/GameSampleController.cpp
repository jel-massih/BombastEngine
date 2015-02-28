#include "GameSampleController.h"
#include "GameSampleEvents.h"
#include "../Graphics3D/SceneNode.h"

const float ACTOR_ACCELERATION = 1.0f;
const float ACTOR_ANGULAR_ACCELERATION = 22.0f;

GameSampleController::GameSampleController(SceneNode* object)
	:m_pObject(object)
{
	memset(m_bKey, 0x00, sizeof(m_bKey));
}

bool GameSampleController::VOnMouseDown(const Point& mousePos, const int radius, const std::string& buttonName)
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

void GameSampleController::OnUpdate(const float deltaMs)
{

}

bool GameSampleController::VOnKeyDown(const BYTE c)
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

bool GameSampleController::VOnKeyUp(const BYTE c)
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
		std::shared_ptr<EvtData_EndRight> pEvent(BE_NEW EvtData_EndRight(actorId));
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