#include "Actor.h"
#include "ActorFactory.h"
#include "ActorComponent.h"
#include "../Bombast/interfaces.h"
#include "../Utilities/templates.h"
#include "../Resources/XmlResource.h"
#include "../Bombast/BombastApp.h"

ActorFactory::ActorFactory()
{
	m_lastActorId = INVALID_ACTOR_ID;
}

Actor* ActorFactory::CreateActor(const char* actorResource, rapidxml::xml_node<>* overrides)
{
	//Get root xml node
	rapidxml::xml_node<>* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(actorResource);
	if (!pRoot)
	{
		BE_ERROR("ERROR: Failed to create actor from resource: " + std::string(actorResource));
		return NULL;
	}

	//Create Actor Instance
	ActorId nextActorId = GetNextActorId();
	Actor* pActor = BE_NEW Actor(nextActorId);
	if (!pActor->Initialize())
	{
		BE_ERROR("ERROR: Failed to initialize Actor: " + std::string(actorResource));
		return NULL;
	}

	//loop through child nodes and load the component
	for (rapidxml::xml_node<>* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling())
	{
		ActorComponent* pComponent = VCreateComponent(pNode);
		if (pComponent)
		{
			pActor->AddComponent(pComponent);
			pComponent->SetOwner(pActor);
		}
		else
		{
			return NULL;
		}
	}

	if (overrides)
	{
		ModifyActor(pActor, overrides);
	}

	return pActor;
}

ActorComponent* ActorFactory::VCreateComponent(rapidxml::xml_node<>* overrides)
{
	return NULL;
}

void ActorFactory::ModifyActor(Actor* pActor, rapidxml::xml_node<>* overrides)
{

}