#pragma once

#include "EventManager.h"
#include "../Bombast/BombastApp.h"

class EvtData_New_Render_Component : public BaseEventData
{
	ActorId m_actorId;
	SceneNode* m_pSceneNode;

public:
	static const EventType sk_EventType;

	EvtData_New_Render_Component()
	{
		m_actorId = INVALID_ACTOR_ID;
	}

	explicit EvtData_New_Render_Component(ActorId actorId, SceneNode* pSceneNode)
		:m_actorId(actorId),
		m_pSceneNode(pSceneNode)
	{
	}

	virtual void VSerialize(std::ostrstream& out) const
	{
		BE_ERROR(GetName() + std::string(" should not be serialized!"));
	}

	virtual void VDeserialize(std::istrstream& in)
	{
		BE_ERROR(GetName() + std::string(" should not be Deserialized!"));
	}

	virtual const EventType& VGetEventType() const
	{
		return sk_EventType;
	}

	virtual IEventData* VCopy() const
	{
		return BE_NEW EvtData_New_Render_Component(m_actorId, m_pSceneNode);
	}

	virtual const char* GetName() const
	{
		return "EvtData_New_Render_Component";
	}

	const ActorId GetActorId() const
	{
		return m_actorId;
	}

	SceneNode* GetSceneNode() const
	{
		return m_pSceneNode;
	}
};