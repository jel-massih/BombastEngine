#ifndef INTERFACES_H
#define INTERFACES_H

#include "..\Utilities\rapidxml.hpp"
#include "../Graphics3D/Geometry.h"
#include <string>

class Actor;

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

class IGameLogic
{
public:
	virtual Actor* VGetActor(const ActorId id) = 0;
	virtual Actor* VCreateActor(const std::string &actorResource, rapidxml::xml_node<> *overrides) = 0;
	virtual void VDestroyActor(const ActorId actorId) = 0;
	virtual bool VLoadGame(const char* levelResource) = 0;
	virtual void VChangeState(enum CoreGameState newState) = 0;
};


class ResourceHandle;
class Resource;

class IResourceLoader
{
public:
	virtual std::string VGetPattern() = 0;
	virtual bool VUseRawFile() = 0;
	virtual bool VDiscardRawBufferAfterLoad() = 0;
	virtual bool VAddNullZero() { return false; }
	virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) = 0;
	virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, ResourceHandle* handle) = 0;
};

class IResourceFile
{
public:
	virtual bool VOpen() = 0;
	virtual int VGetRawResourceSize(const Resource &r) = 0;
	virtual int VGetRawResource(const Resource &r, char *buffer) = 0;
	virtual int VGetNumResources() const = 0;
	virtual std::string VGetResourceName(int num) const = 0;
	virtual ~IResourceFile() { }
};

enum RenderPass
{
	RenderPass_0,
	RenderPass_Static = RenderPass_0,
	RenderPass_Actor,
	RenderPass_Sky,
	RenderPass_NotRendered,
	RenderPass_Last
};

class Scene;
class SceneNodeProperties;
class RayCast;

class IRenderState
{
public:
	virtual std::string VToString() = 0;
};

class IRenderer
{
public:
	virtual bool VInitialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool bFullscreen, float screenDepth, float screenNear) = 0;
	virtual void VShutdown() = 0;
	virtual void VSetBackgroundColor(BYTE a, BYTE r, BYTE g, BYTE b) = 0;
	virtual void VBeginScene() = 0;
	virtual void VEndScene() = 0;

	virtual HRESULT VOnRestore() = 0;

	virtual void VSetWorldTransform(const Mat4x4* m) = 0;
	virtual void VSetOrthoTransform(const Mat4x4* m) = 0;
	virtual void VSetProjectionTransform(const Mat4x4* m) = 0;
	virtual void VSetViewTransform(const Mat4x4* m) = 0;

	virtual void VGetWorldMatrix(Mat4x4& m) = 0;
	virtual void VGetOrthoMatrix(Mat4x4& m) = 0;
	virtual void VGetProjectionMatrix(Mat4x4& m) = 0;
	virtual void VGetViewMatrix(Mat4x4& m) = 0;

	virtual void VGetVideoCardInfo(char* cardName, int& memory) = 0;
	virtual void VEnableZBuffer(bool bEnable) = 0;
	virtual void VEnableAlphaBlending(bool bEnable) = 0;
	virtual void VToggleFillMode() = 0;


	//@TODO: Remove D3D Specific Method
	virtual ID3D11Device* GetDevice() = 0;
	virtual ID3D11DeviceContext* GetDeviceContext() = 0;
};

class ISceneNode
{
public:
	virtual const SceneNodeProperties* const VGet() const = 0;

	virtual void VSetTransform(const Mat4x4* toWorld, const Mat4x4* fromWorld = NULL) = 0;

	virtual HRESULT VOnUpdate(Scene* pScene, DWORD const elapsedMs) = 0;
	virtual HRESULT VOnRestore(Scene* pScene) = 0;

	virtual HRESULT VPreRender(Scene* pScene) = 0;
	virtual bool VIsVisible(Scene* pScene) const = 0;
	virtual HRESULT VRender(Scene* pScene) = 0;
	virtual HRESULT VRenderChildren(Scene* pScene) = 0;
	virtual HRESULT VPostRender(Scene* pScene) = 0;

	virtual bool VAddChild(ISceneNode* kid) = 0;
	virtual bool VRemoveChild(ActorId id) = 0;
	virtual HRESULT VOnLostDevice(Scene* pScene) = 0;
	virtual HRESULT VPick(Scene* pScene, RayCast* pRayCast)=0;

	virtual ~ISceneNode() {};
};

#endif