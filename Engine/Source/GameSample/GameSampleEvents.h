#pragma once

#include <memory>

#include "../Events/EventManager.h"
#include "../Events/Events.h"

class EvtData_Fire_Weapon : public BaseEventData
{
	ActorId m_id;

public:
	static const EventType sk_EventType;
	virtual const EventType& VGetEventType() const
	{
		return sk_EventType;
	}

	EvtData_Fire_Weapon()
	{
		m_id = INVALID_ACTOR_ID;
	}
	
	EvtData_Fire_Weapon(ActorId actorId)
		:m_id(actorId)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_Fire_Weapon(m_id));
	}

	virtual void VSerialize(std::ostrstream& out) const
	{
		out << m_id << " ";
	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_id;
	}

	virtual const char* GetName() const
	{
		return "EvtData_Fire_Weapon";
	}

	ActorId GetActorId() const
	{
		return m_id;
	}
};

class BaseMovementStartEvent : public BaseEventData
{
protected:
	ActorId m_id;
	float m_acceleration;

public:
	BaseMovementStartEvent() :
		m_id(INVALID_ACTOR_ID),
		m_acceleration(0)
	{
	}

	BaseMovementStartEvent(ActorId actorId, float acceleration) :
		m_id(actorId),
		m_acceleration(acceleration)
	{
	}

	virtual void VSerialize(std::ostrstream& out) const
	{
		out << m_id << " ";
		out << m_acceleration << " ";
	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_id;
		in >> m_acceleration;
	}

	ActorId GetActorId() const
	{
		return m_id;
	}

	float GetAcceleration() const
	{
		return m_acceleration;
	}

	void Set(ActorId id)
	{
		m_id = id;
	}
};

class BaseMovementEndEvent : public BaseEventData
{
protected:
	ActorId m_id;

public:
	BaseMovementEndEvent(void) :
		m_id(INVALID_ACTOR_ID)
	{
	}

	BaseMovementEndEvent(ActorId id) :
		m_id(id)
	{
	}



	virtual void VSerialize(std::ostrstream & out) const
	{
		out << m_id << " ";
	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_id;
	}

	ActorId GetActorId(void) const
	{
		return m_id;
	}

	void Set(ActorId id)
	{
		m_id = id;
	}
};

class EvtData_StartUp : public BaseMovementStartEvent
{
public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_StartUp(ActorId actorId, float acceleration) :
		BaseMovementStartEvent(actorId, acceleration)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_StartUp(m_id, m_acceleration));
	}

	virtual const char* GetName() const
	{
		return "EvtData_StartUp";
	}
};

class EvtData_EndUp : public BaseMovementEndEvent
{
public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_EndUp(ActorId actorId) :
		BaseMovementEndEvent(actorId)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_EndUp(m_id));
	}
	virtual const char* GetName(void) const
	{
		return "EvtData_EndUp";
	}
};

class EvtData_StartRight : public BaseMovementStartEvent
{
public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_StartRight(ActorId actorId, float acceleration) :
		BaseMovementStartEvent(actorId, acceleration)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_StartRight(m_id, m_acceleration));
	}

	virtual const char* GetName() const
	{
		return "EvtData_StartRight";
	}
};

class EvtData_EndRight : public BaseMovementEndEvent
{
public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_EndRight(ActorId actorId) :
		BaseMovementEndEvent(actorId)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_EndRight(m_id));
	}
	virtual const char* GetName(void) const
	{
		return "EvtData_EndRight";
	}
};

class EvtData_StartForward : public BaseMovementStartEvent
{
public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_StartForward(ActorId actorId, float acceleration) :
		BaseMovementStartEvent(actorId, acceleration)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_StartForward(m_id, m_acceleration));
	}

	virtual const char* GetName() const
	{
		return "EvtData_StartForward";
	}
};

class EvtData_EndForward : public BaseMovementEndEvent
{
public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_EndForward(ActorId actorId) :
		BaseMovementEndEvent(actorId)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_EndForward(m_id));
	}
	virtual const char* GetName(void) const
	{
		return "EvtData_EndForward";
	}
};

class EvtData_StartTurnRight : public BaseMovementStartEvent
{
public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_StartTurnRight(ActorId actorId, float acceleration) :
		BaseMovementStartEvent(actorId, acceleration)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_StartTurnRight(m_id, m_acceleration));
	}

	virtual const char* GetName() const
	{
		return "EvtData_StartTurnRight";
	}
};

class EvtData_EndTurnRight : public BaseMovementEndEvent
{
public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_EndTurnRight(ActorId actorId) :
		BaseMovementEndEvent(actorId)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_EndTurnRight(m_id));
	}
	virtual const char* GetName(void) const
	{
		return "EvtData_EndTurnRight";
	}
};

class EvtData_StartTurnUp : public BaseMovementStartEvent
{
public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_StartTurnUp(ActorId actorId, float acceleration) :
		BaseMovementStartEvent(actorId, acceleration)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_StartTurnUp(m_id, m_acceleration));
	}

	virtual const char* GetName() const
	{
		return "EvtData_StartTurnUp";
	}
};

class EvtData_EndTurnUp : public BaseMovementEndEvent
{
public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_EndTurnUp(ActorId actorId) :
		BaseMovementEndEvent(actorId)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_EndTurnUp(m_id));
	}
	virtual const char* GetName(void) const
	{
		return "EvtData_EndTurnLeft";
	}
};

class EvtData_Set_Controlled_Actor : public BaseEventData
{
	ActorId m_id;

public:
	static const EventType sk_EventType;

	EvtData_Set_Controlled_Actor(void) { }
	EvtData_Set_Controlled_Actor(ActorId actorId)
		: m_id(actorId)
	{
	}

	virtual const EventType& VGetEventType(void) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_Set_Controlled_Actor(m_id));
	}

	virtual void VSerialize(std::ostrstream& out) const
	{
		out << m_id;
	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_id;
	}

	const ActorId& GetActorId(void) const
	{
		return m_id;
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_Set_Controlled_Actor";
	}
};

class EvtData_SpawnPlayer : public BaseEventData
{
	ActorId m_id;
	GameViewId m_viewId;

public:
	static const EventType sk_EventType;

	EvtData_SpawnPlayer(void) { }
	EvtData_SpawnPlayer(ActorId actorId, GameViewId viewId)
		: m_id(actorId), m_viewId(viewId)
	{
	}

	virtual const EventType& VGetEventType(void) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_SpawnPlayer(m_id, m_viewId));
	}

	virtual void VSerialize(std::ostrstream& out) const
	{
		out << m_id << " ";
		out << m_viewId << " ";
	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_id;
		in >> m_viewId;
	}

	const ActorId& GetActorId(void) const
	{
		return m_id;
	}

	const GameViewId& GetViewId(void) const
	{
		return m_viewId;
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_SpawnPlayer";
	}
};