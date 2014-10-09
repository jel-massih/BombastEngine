#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <strstream>
#include "../MultiThreading/MultiThreadUtilities.h"
#include "../Utilities/FastDelegate/FastDelegate.h"

typedef unsigned long EventType;
typedef fastdelegate::FastDelegate1<IEventData*> EventListenerDelegate;
typedef concurrent_queue<IEventData*> ThreadSafeEventQueue;

class IEventData
{
public:
	virtual ~IEventData(void){}
	virtual const EventType& VGetEventType(void) const = 0;
	virtual float GetTimeStamp(void) const = 0;
	virtual void VSerialize(std::ostrstream& out) const = 0;
	virtual void VDeserialize(std::istrstream& in) const = 0;
	virtual IEventData* VCopy(void) const = 0;
	virtual const char* GetName(void) const = 0;
};

class BaseEventData : IEventData {
}

#endif