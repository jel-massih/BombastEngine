#include "BpPhysicsCore.h"
#include "Scene\BpScene.h"

using namespace bPhysics;

BpPhysicsCore::BpPhysicsCore()
	: m_pScene(nullptr)
{

}

BpPhysicsCore::~BpPhysicsCore()
{

}

BpScene* BpPhysicsCore::CreateScene(const BpSceneDesc& sceneDesc)
{
	m_pScene = new BpScene;

	m_pScene->InitFromSceneDesc(sceneDesc);
	
	return m_pScene;
}