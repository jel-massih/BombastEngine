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
		return "EvtData_FIre_Weapons";
	}

	ActorId GetActorId() const
	{
		return m_id;
	}
};

class EvtData_StartThrust : public BaseEventData
{
	ActorId m_id;
	float m_acceleration;

public:
	static const EventType sk_EventType;
	virtual const EventType& VGetEventType() const
	{
		return sk_EventType;
	}

	EvtData_StartThrust() :
		m_id(INVALID_ACTOR_ID),
		m_acceleration(0)
	{
	}

	EvtData_StartThrust(ActorId actorId, float acceleration) :
		m_id(actorId),
		m_acceleration(acceleration)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_StartThrust(m_id, m_acceleration));
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
		return "EvtData_ThrustStart";
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

class EvtData_EndThrust : public BaseEventData
{
	ActorId m_id;

public:
	static const EventType sk_EventType;
	virtual const EventType & VGetEventType(void) const
	{
		return sk_EventType;
	}

	EvtData_EndThrust(void) :
		m_id(INVALID_ACTOR_ID)
	{
	}

	EvtData_EndThrust(ActorId id) :
		m_id(id)
	{
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_EndThrust(m_id));
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
		return "EvtData_EndThrust";
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