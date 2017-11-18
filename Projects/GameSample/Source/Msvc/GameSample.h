#pragma once

namespace physx
{
	class PxControllerManager;
}

class GameSampleApp : public BombastApp
{
protected:
	virtual CoreGameLogic *VCreateGameAndView();

public:
	virtual TCHAR *VGetGameTitle() { return _T("Bombast Engine Sample Game"); }
	//virtual TCHAR *VGetGameAppDirectory() { return _T(""); }

protected:
};


class GameSampleLogic : public CoreGameLogic
{
protected:
	physx::PxControllerManager* m_pControllerManager;

public:
	GameSampleLogic();
	virtual ~GameSampleLogic();

	virtual void VMoveActor(const ActorId id, Mat4x4 const &mat);

	virtual void VChangeState(CoreGameState newState);

	void RequestStartGameDelegate(EventDataPtr pEventData);
	void StartMoveUpDelegate(EventDataPtr pEventData);
	void EndMoveUpDelegate(EventDataPtr pEventData);
	void StartMoveRightDelegate(EventDataPtr pEventData);
	void EndMoveRightDelegate(EventDataPtr pEventData);
	void StartMoveForwardDelegate(EventDataPtr pEventData);
	void EndMoveForwardDelegate(EventDataPtr pEventData);

	void StartTurnRightDelegate(EventDataPtr pEventData);
	void EndTurnRightDelegate(EventDataPtr pEventData);
	void StartTurnUpDelegate(EventDataPtr pEventData);
	void EndTurnUpDelegate(EventDataPtr pEventData);

protected:
	virtual bool VLoadGameDelegate(rapidxml::xml_node<>* pLevelData);

private:
	void RegisterAllDelegates();
	void RemoveAllDelegates();
};