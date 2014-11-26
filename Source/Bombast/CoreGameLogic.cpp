#include "CoreGameLogic.h"
#include "BombastApp.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorFactory.h"
#include "../Utilities/String.h"
#include "../Resources/ResourceCache.h"
#include "../Resources/XmlResource.h"
#include "../Resources/LuaResource.h"
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
	
	m_pLevelManager->Initialize(g_pApp->m_pResourceCache->Match("world\\*.xml"));
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
	if (!g_pApp->m_options.m_level.empty())
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
	Actor* pActor = m_pActorFactory->CreateActor(actorResource.c_str(), overrides);
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
		SAFE_DELETE(target->second);
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

	//Run all Pre Init Scripts
	rapidxml::xml_node<>* pPreLoadNode = pRoot->first_node("PreLoadScripts");
	if (pPreLoadNode)
	{
		for (rapidxml::xml_node<>* pNode = pPreLoadNode->first_node(); pNode; pNode = pNode->next_sibling())
		{
			const char* scriptResource = pNode->first_attribute("resource")->value();
			
			LuaResourceLoader::LoadLuaScript(scriptResource);
		}
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

	//call delegate load
	if (!VLoadGameDelegate(pRoot)) 
	{
		return false;
	}

	//Run all Post Init Scripts
	rapidxml::xml_node<>* pPostLoadNode = pRoot->first_node("PostLoadScripts");
	if (pPostLoadNode)
	{
		for (rapidxml::xml_node<>* pNode = pPostLoadNode->first_node(); pNode; pNode = pNode->next_sibling())
		{
			const char* scriptResource = pNode->first_attribute("resource")->value();
			
			g_pApp->GetLuaCoreManager()->LoadScript(scriptResource);
		}
	}

	return true;
}

void CoreGameLogic::VChangeState(enum CoreGameState newState)
{
	if (newState == CGS_LoadingGameEnvironment)
	{
		m_gameState = newState;
		if (!g_pApp->VLoadGame())
		{
			BE_ERROR("ERROR: Game failed to load!");
		}
	}
	m_gameState = newState;
}

ActorFactory* CoreGameLogic::VCreateActorFactory()
{
	return BE_NEW ActorFactory();
}

void CoreGameLogic::VAddView(IGameView* pView, ActorId actorId)
{
	int viewId = static_cast<int>(m_gameViews.size());
	m_gameViews.push_back(pView);
	pView->VOnAttach(viewId, actorId);
	pView->VOnRestore();
}

void CoreGameLogic::VRemoveView(IGameView* pView)
{
	m_gameViews.remove(pView);
}
