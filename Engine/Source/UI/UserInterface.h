#pragma once
#include "..\Graphics3D\Scene.h"

class ScreenElementScene : public IScreenElement, public Scene
{
public:
	ScreenElementScene(IRenderer* renderer) : Scene(renderer) {}
	virtual ~ScreenElementScene() {}

	virtual void VOnUpdate(const float deltaMs) { OnUpdate(deltaMs); }
	virtual HRESULT VOnRestore() { OnRestore(); return S_OK; }
	virtual HRESULT VOnRender(const float deltaMs, double elapsedMs) { return DoRender(); }
	
	virtual HRESULT VOnLostDevice() { OnLostDevice(); return S_OK; }
	virtual int VGetZOrder() const { return 0; }
	virtual void VSetZOrder(int const zOrder) {}

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) { return 0; }

	virtual bool VIsVisible() const { return true; }
	virtual void VSetVisible(bool visible) {}
	virtual bool VAddChild(ActorId id, ISceneNode* kid) { return Scene::AddChild(id, kid); }
};