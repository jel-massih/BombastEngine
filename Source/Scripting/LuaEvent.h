#ifndef LUA_EVENT_H
#define LUA_EVENT_H

class ScriptEvent;
typedef ScriptEvent* (*CreateEventForScriptFunctionType)(void); //function ptr typedef to create lua event
//Export events to lua macros

#endif