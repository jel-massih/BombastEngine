#include "CoreGameLogic.h"
#include "BombastApp.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorFactory.h"
#include "../Utilities/String.h"
#include "../Resources/ResourceCache.h"
#include "../Resources/XmlResource.h"
#include <fstream>
#include <sstream>

bool LevelManager::Initialize(std::vector<std::string> &levels)
{
	std::vector<std::string>::iterator i = levels.begin();
	while (i != levels.end())
	{
		m_levels.push_back(*i);
		i++;
	}

	return true;
}

CoreGameLogic::CoreGameLogic()
{
	m_lastActorId = 0;
	m_lifetime = 0;
	m_gameState = CGS_Initializing;
	m_pActorFactory = NULL;

	m_pLevelManager = BE_NEW LevelManager;
	
	m_pLevelManager->Initialize(BombastApp::GetGameInstance()->m_pResourceCache->Match("world\\*.xml"));
}

CoreGameLogic::~CoreGameLogic()
{
	SAFE_DELETE(m_pLevelManager);
	SAFE_DELETE(m_pActorFactory);

	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		it->second->Shutdown();
		SAFE_DELETE(it->second);
	}
	m_actors.clear();
}

bool CoreGameLogic::Initialize()
{
	m_pActorFactory = VCreateActorFactory();
	if (!BombastApp::GetGameInstance()->m_options.m_level.empty())
	{
		VChangeState(CGS_LoadingGameEnvironment);
	}

	return true;
}

std::string CoreGameLogic::GetActorXml(const ActorId actorId)
{
	Actor* pActor = VGetActor(actorId);
	if (pActor)
	{
		return pActor->ToXML();
	}
	else
	{
		BE_ERROR("Couldnt find actor: " + actorId);
	}

	return "";
}

Actor* CoreGameLogic::VGetActor(const ActorId id)
{
	ActorMap::iterator target = m_actors.find(id);

	if (target != m_actors.end())
	{
		return target->second;
	}

	return NULL;
}

Actor* CoreGameLogic::VCreateActor(const std::string &actorResource, rapidxml::xml_node<> *overrides)
{
	BE_ASSERT(m_pActorFactory);
	Actor* pActor = NULL;
	//Actor* pActor = m_pActorFactory->CreateActor(actorResource.c_str(), overrides);
	if (pActor)
	{
		m_actors.insert(std::make_pair(pActor->GetId(), pActor));
		return pActor;
	}
	else
	{
		BE_ERROR("ERROR: Failed To Create Actor From resource: " + actorResource);
		return NULL;
	}
}

void CoreGameLogic::VDestroyActor(const ActorId actorId)
{
	auto target = m_actors.find(actorId);

	if (target != m_actors.end())
	{
		target->second->Shutdown();
		delete target->second;
		m_actors.erase(target);
	}
}

bool CoreGameLogic::VLoadGame(const char* levelResource)
{
	//Get Root node
	rapidxml::xml_node<> *pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(levelResource);
	if (!pRoot)
	{
		BE_ERROR("ERROR: Cannot find Level resource file: " + std::string(levelResource));
		return false;
	}

	// Load All Initial Actors
	rapidxml::xml_node<>* pActorsNode = pRoot->first_node("StaticActors");
	if (pActorsNode)
	{
		for (rapidxml::xml_node<>* pNode = pActorsNode->first_node(); pNode; pNode = pNode->next_sibling())
		{
			const char* actorResource = pNode->first_attribute("resource")->value();

			Actor* pActor = VCreateActor(actorResource, pNode);
		}
	}

	return true;
}

void CoreGameLogic::VChangeState(enum CoreGameState newState)
{
	if (newState == CGS_LoadingGameEnvironment)
	{
		m_gameState = newState;
		if (!BombastApp::GetGameInstance()->VLoadGame())
		{
			BE_ERROR("ERROR: Game failed to load!");
		}
		else
		{
			VChangeState(CGS_SpawningPlayersActors);
			return;
		}
	}
	m_gameState = newState;
}

ActorFactory* CoreGameLogic::VCreateActorFactory()
{
	return BE_NEW ActorFactory();
}