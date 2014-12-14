#pragma once

class PongSampleApp : public BombastApp
{
protected:
	virtual CoreGameLogic *VCreateGameAndView();

public:
	virtual TCHAR *VGetGameTitle() { return _T("Pong Sample"); }
	//virtual TCHAR *VGetGameAppDirectory() { return _T(""); }

protected:
};


class PongSampleLogic : public CoreGameLogic
{
protected:

public:
	PongSampleLogic();
	virtual ~PongSampleLogic();

	virtual void VMoveActor(const ActorId id, Mat4x4 const &mat);

	virtual void VChangeState(CoreGameState newState);

	void RequestStartGameDelegate(IEventDataPtr pEventData);

protected:
	virtual bool VLoadGameDelegate(rapidxml::xml_node<>* pLevelData);
};