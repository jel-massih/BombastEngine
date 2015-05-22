#pragma once

#include "..\Utilities\rapidxml.hpp"
#include <list>
#include <vector>

class Actor;
class Vec4;
class Vec3;

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

template <class T>
struct SortBy_Ptr_Content
{
	bool operator()(const T* lhs, const T* rhs) const
	{
		return lhs < rhs;
	}
};

class IScreenElement
{
public:
	virtual HRESULT VOnRestore() = 0;
	virtual HRESULT VOnLostDevice() = 0;
	virtual HRESULT VOnRender(const float deltaMs, double elapsedMs) = 0;
	virtual void VOnUpdate(const float deltaMs) = 0;

	virtual int VGetZOrder() const = 0;
	virtual void VSetZOrder(int const zOrder) = 0;
	virtual bool VIsVisible() const = 0;
	virtual void VSetVisible(bool bVisible) = 0;

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
	
	virtual ~IScreenElement() {};
	virtual bool const operator<(IScreenElement const& other) { return VGetZOrder() < other.VGetZOrder(); }
};

class IGameLogic
{
public:
	virtual Actor* VGetActor(const ActorId id) = 0;
	virtual Actor* VCreateActor(const std::string &actorResource, rapidxml::xml_node<> *overrides) = 0;
	virtual void VDestroyActor(const ActorId actorId) = 0;
	virtual bool VLoadGame(const char* levelResource) = 0;
	virtual void VChangeState(enum CoreGameState newState) = 0;
};

enum GameViewType
{
	GameView_Human,
	GameView_Remote,
	GameView_AI,
	GameView_Other
};

typedef unsigned int GameViewId;
extern const GameViewId be_InvalidGameViewId;

class IGameView
{
public:
	virtual HRESULT VOnRestore() = 0;
	virtual void VOnRender(const float deltaMs, double elapsedMs) = 0;
	virtual HRESULT VOnLostDevice() = 0;
	virtual GameViewType VGetType() = 0;
	virtual GameViewId VGetId() const = 0;
	virtual void VOnAttach(GameViewId vid, ActorId aid) = 0;

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
	virtual void VOnUpdate(const float deltaMs) = 0;

	virtual ~IGameView(){};
};

typedef std::list<IGameView*> GameViewList;
typedef std::list<IScreenElement*> ScreenElementList;

class IKeyboardHandler
{
public:
	virtual bool VOnKeyDown(const BYTE c) = 0;
	virtual bool VOnKeyUp(const BYTE c) = 0;
};

class IMouseHandler
{
public:
	virtual bool VOnMouseMove(const Point& pos, const int radius) = 0;
	virtual bool VOnMouseDown(const Point& pos, const int radius, const std::string& buttonName) = 0;
	virtual bool VOnMouseUp(const Point& pos, const int radius, const std::string& buttonName) = 0;
};

class ResourceHandle;
class Resource;

class IResourceLoader
{
public:
	virtual std::vector<std::string> VGetPatterns() = 0;
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
	virtual size_t VGetRawResource(const Resource &r, char *buffer) = 0;
	virtual size_t VGetNumResources() const = 0;
	virtual std::string VGetResourceName(size_t num) const = 0;
	virtual ~IResourceFile() { }
};

enum RenderPass
{
	RenderPass_0,
	RenderPass_Static = RenderPass_0,
	RenderPass_Actor,
	RenderPass_Sky,
	RenderPass_GUI,
	RenderPass_NotRendered,
	RenderPass_Last
};

enum ShaderType {
	BSHADER_TYPE_INVALID = -1,
	BSHADER_TYPE_COLOR,
	BSHADER_TYPE_FONT,
	BSHADER_TYPE_LIT,
	BSHADER_TYPE_DEFERRED_LIT
};

class Scene;
class SceneNodeProperties;
class RayCast;
class LightNode;

class IRenderState
{
public:
	virtual std::string VToString() = 0;
};

class IRenderer
{
public:
	virtual ~IRenderer() {}
	virtual bool VInitialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool bFullscreen, float screenDepth, float screenNear) = 0;
	virtual void VSetBackgroundColor(float r, float g, float b, float a) = 0;
	virtual bool VBeginScene() = 0;
	virtual bool VEndScene() = 0;

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

	virtual void VToggleFullscreen() = 0;

	virtual void VCalcLighting(std::list<LightNode*>* lights, int maxLights) = 0;

	virtual void VPrepDeferredRendering() const = 0;
	virtual void VPrepForwardRendering() const = 0;

	//@TODO: Remove D3D Specific Method
	virtual ID3D11Device* GetDevice() = 0;
	virtual ID3D11DeviceContext* GetDeviceContext() = 0;
};

class ISceneNode
{
public:
	virtual const SceneNodeProperties* const VGet() const = 0;

	virtual void VSetTransform(const Mat4x4* toWorld, const Mat4x4* fromWorld = NULL) = 0;

	virtual HRESULT VOnUpdate(Scene* pScene, const float deltaMs) = 0;
	virtual HRESULT VOnRestore(Scene* pScene) = 0;

	virtual bool VIsVisible(Scene* pScene) const = 0;
	
	virtual HRESULT VPreRender(Scene* pScene) = 0;
	virtual HRESULT VPostRender(Scene* pScene) = 0;


	virtual HRESULT VDeferredRender(Scene* pScene) = 0;
	virtual HRESULT VDeferredRenderChildren(Scene* pScene) = 0;

	virtual HRESULT VForwardRender(Scene* pScene) = 0;
	virtual HRESULT VForwardRenderChildren(Scene* pScene) = 0;

	virtual bool VAddChild(ISceneNode* kid) = 0;
	virtual bool VRemoveChild(ActorId id) = 0;
	virtual HRESULT VOnLostDevice(Scene* pScene) = 0;
	virtual HRESULT VPick(Scene* pScene, RayCast* pRayCast)=0;

	virtual ~ISceneNode() {};
};

class IDebugPhysicsSphere
{
public:
	virtual Vec3 VGetPosition() = 0;
	virtual Vec3 VGetColor() = 0;
};

class IDebugPhysicsRenderBuffer
{
public:
	virtual std::vector<IDebugPhysicsSphere*> VGetDebugSpheres() = 0;
};

//Generic Physics Interface
class IGamePhysics
{
public:
	virtual bool VInitialize() = 0;
	virtual void VSyncVisibleScene() = 0;
	virtual void VOnUpdate(float deltaMs) = 0;

	virtual void VAddSphere(float radius, Actor* gameActor, const std::string& densityStr, const std::string& physicsMaterial, bool gravityEnabled, float linearDamping, float angularDamping) = 0;
	virtual void VRemoveActor(ActorId actor) = 0;

	virtual void VRenderDiagnostics() = 0;

	virtual void VApplyForce(const Vec3& dir, float newtons, ActorId actor) = 0;
	virtual void VApplyTorque(const Vec3& dir, float newtons, ActorId actor) = 0;
	virtual bool VMove(const Mat4x4& mat, ActorId actor) = 0;

	virtual void VStopActor(ActorId actorId) = 0;
	virtual Vec3 VGetVelocity(ActorId actorId) = 0;
	virtual void VSetVelocity(ActorId actorId, const Vec3& vel) = 0;
	virtual Vec3 VGetAngularVelocity(ActorId actorId) = 0;
	virtual void VSetAngularVelocity(ActorId actorId, const Vec3& vel) = 0;
	virtual void VTranslate(ActorId actorId, const Vec3& vec) = 0;

	virtual void VSetTransform(const ActorId actorId, const Mat4x4& mat) = 0;
	virtual Mat4x4 VGetTransform(const ActorId actorId) = 0;

	virtual IDebugPhysicsRenderBuffer* VGetDebugRenderBuffer() = 0;

	virtual ~IGamePhysics() {};
};