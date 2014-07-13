#include "CoreGameLogic.h"
#include "BombastApp.h"
#include "../Actor/Actor.h"
#include "../Utilities/String.h"

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
	
	//m_pLevelManager->Initialize(BombastApp::GetGameInstance()->m_ResCache->Match("world\\*.xml"));
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
		BE_ERROR(s2ws("Couldnt find actor: " + actorId));
	}

	return "";
}

Actor* CoreGameLogic::VGetActor(const ActorId id)
{
	return NULL;
}

Actor* CoreGameLogic::VCreateActor(const std::string &actorResource, rapidxml::xml_node<> *overrides)
{
	return NULL;
}

void CoreGameLogic::VDestroyActor(const ActorId actorId)
{

}

bool CoreGameLogic::VLoadGame(const char* levelResource)
{
	return true;
}

void CoreGameLogic::VChangeState(enum CoreGameState newState)
{

}

ActorFactory* CoreGameLogic::VCreateActorFactory()
{
	return NULL;
}