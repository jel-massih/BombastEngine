#pragma once

#include "../BombastEngineStd.h"
#include "interfaces.h"
#include <map>
#include <vector>

class ActorFactory;
class LevelManager;

enum CoreGameState
{
	CGS_Invalid,
	CGS_Initializing,
	CGS_MainMenu,
	CGS_LoadingGameEnvironment,
	CGS_SpawningPlayersActors,
	CGS_WaitingForPlayers,
	CGS_Running
};

typedef std::map<ActorId, Actor*> ActorMap;
typedef std::string Level;

class LevelManager
{
public:
	const std::vector<Level> &GetLevels() const { return m_levels; }
	const int GetCurrentLevel() const { return m_currentLevel; }
	bool Initialize(std::vector<std::string> &levels);

protected:
	std::vector<Level> m_levels;
	int m_currentLevel;
};

class CoreGameLogic : public IGameLogic
{
	friend class BombastApp;

public:
	CoreGameLogic();
	virtual ~CoreGameLogic();

	bool Initialize();
	
	ActorId GetNewActorId()
	{
		return ++m_lastActorId;
	}

	virtual Actor* VCreateActor(const std::string &actorResource, rapidxml::xml_node<> *overrides);
	virtual void VDestroyActor(const ActorId actorId);
	virtual Actor* VGetActor(const ActorId actorId);
	virtual void VModifyActor(const ActorId actorId, rapidxml::xml_node<> *overrides);

	std::string GetActorXml(const ActorId id);

	const LevelManager* GetLevelManager() { return m_pLevelManager; }
	virtual bool VLoadGame(const char* levelResource) override;

	virtual void VOnUpdate(const float deltaMs, double elapsedMs);
	virtual void VOnRender(const float deltaMs, double elapsedMs);

	virtual void VChangeState(CoreGameState newState);
	const CoreGameState GetState() const { return m_gameState; }
	
	virtual void VAddView(IGameView* pView, ActorId actorId = INVALID_ACTOR_ID);
	virtual void VRemoveView(IGameView* pView);

	void ToggleRenderDiagnostice() { m_bRenderDiagnostics = !m_bRenderDiagnostics; }
	virtual void VRenderDiagnostics();
	virtual IGamePhysics* VGetGamePhysics() const { return m_pGamePhysics; }

protected:
	virtual ActorFactory* VCreateActorFactory();

	virtual bool VLoadGameDelegate(rapidxml::xml_node<>* pLevelData) { return true; }
	virtual void VLoadWorldProperties(rapidxml::xml_node<>* pWorldProperiesNode);

protected:
	float m_lifetime; //How long game has been active
	ActorMap m_actors;
	ActorId m_lastActorId;
	CoreGameState m_gameState;

	ActorFactory* m_pActorFactory;

	LevelManager* m_pLevelManager;

	GameViewList m_gameViews; 

	bool m_bRenderDiagnostics;
	IGamePhysics* m_pGamePhysics;

	int m_expectedPlayers;
	int m_humanPlayersAttached;
};