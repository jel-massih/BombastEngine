#include "../msvc/stdafx.h"
#include "BpScene.h"

using namespace bPhysics;

BpScene::BpScene()
	: m_gravity(BpVec3::g_InvalidBpVec3),
	m_bounceThresholdVelocity(0)
{
}

BpScene::~BpScene()
{
}

void BpScene::Release()
{
	for (int i = 0; i < m_sceneActors.size(); i++) 
	{
		 BP_SAFE_DELETE(m_sceneActors[i]);
	}

	m_sceneActors.clear();
}

void BpScene::InitFromSceneDesc(const BpSceneDesc& sceneDesc)
{
	SetGravity(sceneDesc.gravity);
	SetBounceThresholdVelocity(sceneDesc.bounceThresholdVelocity);
}

void BpScene::AddActor(BpActor* actor)
{
	m_sceneActors.push_back(actor);
}