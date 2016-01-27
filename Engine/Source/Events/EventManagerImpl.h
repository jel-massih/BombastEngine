#pragma once

#include "EventManager.h"
#include <list>
#include <map>

const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

class EventManager : public IEventManager
{
	typedef std::list<EventListenerDelegate> EventListenerList;
	typedef std::map<EventType, EventListenerList> EventListenerMap;
	typedef std::list<EventDataPtr> EventQueue;

	EventListenerMap m_eventListeners;
	EventQueue m_queues[EVENTMANAGER_NUM_QUEUES];
	int m_activeQueue;

	ThreadSafeEventQueue m_realtimeEventQueue;

public:
	explicit EventManager(const char* pName, bool setAsGlobal);
	virtual ~EventManager();

	virtual bool VAddListener(const EventListenerDelegate& eventDelegate, const EventType& type);
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type);
	
	virtual bool VTriggerEvent(const EventDataPtr& pEvent) const;
	virtual bool VQueueEvent(const EventDataPtr& pEvent);
	virtual bool VThreadSafeQueueEvent(const EventDataPtr& pEvent);
	virtual bool VAbortEvent(const EventType& type, bool allOfType = false);

	virtual bool VUpdate(unsigned long maxMs = kINFINITE);
};