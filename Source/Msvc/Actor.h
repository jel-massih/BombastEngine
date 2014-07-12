#ifndef ACTOR_H
#define ACTOR_H

#include "../BombastEngineStd.h"
#include "../Bombast/interfaces.h"
#include <string>
#include <map>

class TiXmlElement;
typedef std::string ActorType;

class Actor {
public:
	Actor();
	~Actor();

	bool Initialize();
	void Shutdown();

	bool Frame(int deltaMs);

	ActorId GetId() const;
	ActorType GetType() const;
	
	template <class ComponentClass>;
	Componenttype

public:
	typedef std::map<ComponentId, Actor*> ActorComponents;

private:
	ActorId m_id;
	ActorComponents m_components;
	ActorType m_type;
};

#endif