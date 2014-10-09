#ifndef LUA_EVENT_H
#define LUA_EVENT_H

#include "../Events/EventManager.h"
#include <map>

class LuaEvent;
typedef LuaEvent* (*CreateEventForScriptFunctionType)(void); //function ptr typedef to create lua event
typedef std::map<std::string, std::string> LuaObject;

//Export events to lua macros
#define REGISTER_LUA_EVENT(eventClass, eventType) \
	LuaEvent::RegisterEventTypeWithString(#eventClass, eventType); \
	LuaEvent::AddCreationFunction(eventType, &eventClass::CreateEventForScript)

#define EXPORT_FOR_Lua_EVENT(eventClass) \
	public: \
	static LuaEvent* CreateEventForScript(void) \
	{ \
		return new eventClass; \
	}

class LuaEvent : public BaseEventData
{
public:
	LuaEvent() { m_eventDataIsValid = false; }

	LuaObject GetEventData();
	bool SetEventData(LuaObject eventData);

	static void RegisterEventTypeWithString(const char* key, EventType type);
	static void AddCreationFunction(EventType type, CreateEventForScriptFunctionType pCreationFunctionPtr);
	static LuaEvent* CreateEventFromScript(EventType type);

protected:
	LuaObject m_eventData;

	virtual void VBuildEventData();
	virtual bool VBuildEventFromScript();

private:
	typedef std::map<EventType, CreateEventForScriptFunctionType> CreationFunctions;
	static CreationFunctions s_creationFunctions;

	bool m_eventDataIsValid;
};

#endif