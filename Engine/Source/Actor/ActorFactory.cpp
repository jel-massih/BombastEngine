#include "Actor.h"
#include "ActorFactory.h"
#include "ActorComponent.h"
#include "../Bombast/interfaces.h"
#include "../Utilities/templates.h"
#include "../Resources/XmlResource.h"
#include "../Bombast/BombastApp.h"

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"

ActorFactory::ActorFactory()
{
	m_lastActorId = INVALID_ACTOR_ID;

	m_componentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::g_Name));
	m_componentFactory.Register<InvisibleRenderComponent>(ActorComponent::GetIdFromName(InvisibleRenderComponent::g_Name));
	m_componentFactory.Register<BitmapRenderComponent>(ActorComponent::GetIdFromName(BitmapRenderComponent::g_Name));
	m_componentFactory.Register<MeshRenderComponent>(ActorComponent::GetIdFromName(MeshRenderComponent::g_Name));
	m_componentFactory.Register<LightRenderComponent>(ActorComponent::GetIdFromName(LightRenderComponent::g_Name));
	m_componentFactory.Register<PhysicsComponent>(ActorComponent::GetIdFromName(PhysicsComponent::g_Name));
}

Actor* ActorFactory::CreateActor(const char* actorResource, rapidxml::xml_node<>* overrides)
{
	//Get root xml node
	rapidxml::xml_node<>* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(actorResource);
	if (!pRoot)
	{
		BE_ERROR("ERROR: Failed to create actor from resource: %s", actorResource);
		return NULL;
	}
	
	//Create Actor Instance
	ActorId nextActorId = GetNextActorId();
	Actor* pActor = BE_NEW Actor(nextActorId);
	if (!pActor->Initialize(pRoot))
	{
		BE_ERROR("ERROR: Failed to initialize Actor: %s", actorResource);
		SAFE_DELETE(pActor);
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
			if (pActor)
			{
				SAFE_DELETE(pActor);
			}
			return NULL;
		}
	}

	if (overrides)
	{
		ModifyActor(pActor, overrides);
	}

	pActor->PostInitialize();

	return pActor;
}

ActorComponent* ActorFactory::VCreateComponent(rapidxml::xml_node<>* pData)
{
	const char* name = pData->name();
	ActorComponent* pComponent = m_componentFactory.Create(ActorComponent::GetIdFromName(name));

	//initialize compnent if found
	if (pComponent)
	{
		if (!pComponent->VInitialize(pData))
		{
			BE_ERROR("ERROR: Component Failed to initialize: %s", name);
			return NULL;
		}
	}
	else
	{
		BE_ERROR("ERROR: Couldnt find ActorComponent with name: %s", name);
		return NULL;
	}

	return pComponent;
}

void ActorFactory::ModifyActor(Actor* pActor, rapidxml::xml_node<>* overrides)
{
	//Loop theough each child and load component
	for (rapidxml::xml_node<>* pNode = overrides->first_node(); pNode; pNode = pNode->next_sibling())
	{
		ComponentId componentId = ActorComponent::GetIdFromName(pNode->name());
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