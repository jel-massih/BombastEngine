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

class EvtData_StartUp : public BaseEventData
{
	ActorId m_id;
	float m_acceleration;

public:
	static const EventType sk_EventType;
	virtual const EventType& VGetEventType() const
	{
		return sk_EventType;
	}

	EvtData_StartUp() :
		m_id(INVALID_ACTOR_ID),
		m_acceleration(0)
	{
	}

	EvtData_StartUp(ActorId actorId, float acceleration) :
		m_id(actorId),
		m_acceleration(acceleration)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_StartUp(m_id, m_acceleration));
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

	virtual const char* GetName() const
	{
		return "EvtData_StartUp";
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

class EvtData_EndUp : public BaseEventData
{
	ActorId m_id;

public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_EndUp(void) :
		m_id(INVALID_ACTOR_ID)
	{
	}

	EvtData_EndUp(ActorId id) :
		m_id(id)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_EndUp(m_id));
	}

	virtual void VSerialize(std::ostrstream & out) const
	{
		out << m_id << " ";
	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_id;
	}

	virtual const char* GetName(void) const
	{
		return "EvtData_EndUp";
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