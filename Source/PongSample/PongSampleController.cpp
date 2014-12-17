#include "PongSampleController.h"
#include "PongSampleEvents.h"
#include "../Graphics3D/SceneNode.h"

const float ACTOR_ACCELERATION = 6.5f * 8.0f;
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
		std::shared_ptr<EvtData_StartUp> pEvent(BE_NEW EvtData_StartUp(actorId, (c == 'W' ? ACTOR_ACCELERATION : (-ACTOR_ACCELERATION))));
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
		std::shared_ptr<EvtData_EndUp> pEvent(BE_NEW EvtData_EndUp(actorId));
		IEventManager::Get()->VQueueEvent(pEvent);
	}

	return true;
}