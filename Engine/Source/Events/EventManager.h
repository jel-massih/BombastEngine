#pragma once

#include <strstream>
#include "../MultiThreading/MultiThreadUtilities.h"
#include "../Utilities/FastDelegate/FastDelegate.h"

class IEventData;

typedef unsigned long EventType;
typedef std::shared_ptr<IEventData> EventDataPtr;
typedef fastdelegate::FastDelegate1<EventDataPtr> EventListenerDelegate;
typedef concurrent_queue<EventDataPtr> ThreadSafeEventQueue;

class IEventData
{
public:
	virtual ~IEventData(void){}
	virtual const EventType& VGetEventType() const = 0;
	virtual float GetTimeStamp() const = 0;
	virtual void VSerialize(std::ostrstream& out) const = 0;
	virtual void VDeserialize(std::istrstream& in) = 0;
	virtual EventDataPtr VCopy() const = 0;
	virtual const char* GetName() const = 0;
};

class BaseEventData : public IEventData
{
public:
	explicit BaseEventData(const float timestamp = 0.0f) : m_timestamp(timestamp) {}

	virtual const EventType& VGetEventType() const = 0;

	float GetTimeStamp() const { return m_timestamp; }

	//Serializing for Network input/output
	virtual void VSerialize(std::ostrstream& out) const {}
	virtual void VDeserialize(std::istrstream& in) {}

private:
	const float m_timestamp;
};

class IEventManager
{
public:
	enum eConstants {kINFINITE = 0xffffffff};

	explicit IEventManager(const char* pName, bool setAsGlobal);
	virtual ~IEventManager();

	virtual bool VAddListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

	virtual bool VTriggerEvent(const EventDataPtr& pEvent) const = 0;

	virtual bool VQueueEvent(const EventDataPtr& pEvent) = 0;
	virtual bool VThreadSafeQueueEvent(const EventDataPtr& pEvent) = 0;

	virtual bool VAbortEvent(const EventType& type, bool allOfType = false) = 0;

	virtual bool VUpdate(unsigned long maxMillis = kINFINITE) = 0;

	static IEventManager* Get();
};