#ifndef ACTOR_COMPONENT_H
#define ACTOR_COMPONENT_H

class ActorComponent{
	friend class ActorFactory;

public:
	~ActorComponent() { m_pOwner = 0; };

	//Meant to be overwriten by implementation classes
	virtual bool VInitialize() = 0;
	virtual void VUpdate(int deltaTime){}

	//Overewritten by interface class
	virtual ComponentId VGetId() const { return GetIdFromName(VGetName()); }
	virtual const char* VGetName() const = 0;
	static ComponentId GetIdFromName(const char* componentStr)
	{
		return reinterpret_cast<ComponentId>(componentStr);
	}


protected:
	Actor* m_pOwner;

private:
	void SetOwner(Actor* pOwner) { m_pOwner = pOwner; }
};

#endif