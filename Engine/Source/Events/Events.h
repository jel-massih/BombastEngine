#pragma once

#include "EventManager.h"
#include "../Bombast/BombastApp.h"

class SceneNode;

class EvtData_New_Actor : public BaseEventData
{
	ActorId m_actorId;
	GameViewId m_viewId;

public:
	static const EventType sk_EventType;

	EvtData_New_Actor()
		:m_actorId(INVALID_ACTOR_ID),
		m_viewId(be_InvalidGameViewId)
	{
	}

	explicit EvtData_New_Actor(ActorId actorId, GameViewId viewId = be_InvalidGameViewId)
		:m_actorId(actorId),
		m_viewId(viewId)
	{
	}

	virtual void VSerialize(std::ostrstream& out)
	{
		out << m_actorId << " ";
		out << m_viewId << " ";
	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_actorId;
		in >> m_viewId;
	}

	virtual const EventType& VGetEventType() const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_New_Actor(m_actorId, m_viewId));
	}

	virtual const char* GetName() const
	{
		return "EvtData_New_Actor";
	}

	const ActorId GetActorId() const
	{
		return m_actorId;
	}

	GameViewId GetViewId() const
	{
		return m_viewId;
	}
		
};

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
		BE_ERROR("%s should not be serialized!", GetName());
	}

	virtual void VDeserialize(std::istrstream& in)
	{
		BE_ERROR("%s should not be Deserialized!", GetName());
	}

	virtual const EventType& VGetEventType() const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_New_Render_Component(m_actorId, m_pSceneNode));
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

class EvtData_Request_Start_Game : public BaseEventData
{
public:
	static const EventType sk_EventType;

	EvtData_Request_Start_Game() {}

	virtual const EventType& VGetEventType() const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_Request_Start_Game());
	}

	virtual const char* GetName() const
	{
		return "EvtData_Request_Start_Game";
	}
};

class EvtData_Move_Actor : public BaseEventData
{
	ActorId m_id;
	Mat4x4 m_matrix;

public:
	static const EventType sk_EventType;

	EvtData_Move_Actor() 
		: m_id(INVALID_ACTOR_ID)
	{}

	EvtData_Move_Actor(ActorId id, const Mat4x4& matrix)
		: m_id(id), m_matrix(matrix)
	{
	}

	virtual const EventType& VGetEventType() const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(BE_NEW EvtData_Move_Actor());
	}

	virtual const char* GetName() const
	{
		return "EvtData_Move_Actor";
	}

	ActorId GetId() const { return m_id; }
	const Mat4x4& GetMatrix() const { return m_matrix; }
};