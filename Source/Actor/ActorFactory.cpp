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

ActorComponent* ActorFactory::VCreateComponent(rapidxml::xml_node<>* pData)
{
	const char* name = pData->value();
	ActorComponent* pComponent = m_componentFactory.Create(ActorComponent::GetIdFromName(name));

	//initialize compnent if found
	if (pComponent)
	{
		if (!pComponent->VInitialize(pData))
		{
			BE_ERROR("ERROR: Component Failed to initialize: " + std::string(name));
			return NULL;
		}
	}
	else
	{
		BE_ERROR("ERROR: Couldnt find ActorComponent with name: " + std::string(name));
		return NULL;
	}

	return pComponent;
}

void ActorFactory::ModifyActor(Actor* pActor, rapidxml::xml_node<>* overrides)
{
	//Loop theough each child and load component
	for (rapidxml::xml_node<>* pNode = overrides->first_node(); pNode; pNode = pNode->next_sibling())
	{
		ComponentId componentId = ActorComponent::GetIdFromName(pNode->value());
		ActorComponent* pComponent = pActor->GetComponent<ActorComponent>(componentId);
		if (pComponent)
		{
			pComponent->VInitialize(pNode);
		}
		else
		{
			pComponent = VCreateComponent(pNode);
			if (pComponent)
			{
				pActor->AddComponent(pComponent);
				pComponent->SetOwner(pActor);
			}
		}
	}
}