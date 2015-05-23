#include "../msvc/stdafx.h"
#include "BpScene.h"
#include "../Actors/BpRigidDynamic.h"

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
	for (unsigned int i = 0; i < m_sceneRigidDynamics.size(); i++) 
	{
		BP_SAFE_DELETE(m_sceneRigidDynamics[i]);
	}

	m_sceneRigidDynamics.clear();

	delete this;
}

void BpScene::InitFromSceneDesc(const BpSceneDesc& sceneDesc)
{
	SetGravity(sceneDesc.gravity);
	SetBounceThresholdVelocity(sceneDesc.bounceThresholdVelocity);
}

void BpScene::AddActor(BpActor* actor)
{
	if (actor->GetType() == BpActorType::RIGID_DYNAMIC)
	{
		BpRigidDynamic* body = static_cast<BpRigidDynamic*>(actor);
		m_sceneRigidDynamics.push_back(body);
		body->SetScene(this);

		//Add to visualization
		m_visualizationManager.AddDebugSphere(body->GetWorldTransform().GetPosition(), 50);
	}
}

void BpScene::RemoveActor(BpActor* actor)
{
	if (actor->GetType() == BpActorType::RIGID_DYNAMIC)
	{
		BpRigidDynamic* body = static_cast<BpRigidDynamic*>(actor);
		auto it = std::find(m_sceneRigidDynamics.begin(), m_sceneRigidDynamics.end(), body);
		if (it != m_sceneRigidDynamics.end())
		{
			BP_SAFE_DELETE(*it);
			m_sceneRigidDynamics.erase(it);
		}
	}
}

void BpScene::Simulate(float timestep)
{
	for (auto it = m_sceneRigidDynamics.begin(); it != m_sceneRigidDynamics.end(); it++)
	{
		if (!(*it)->IsSleeping())
		{
			(*it)->Simulate(timestep);
		}
	}
}

void BpScene::SetVisualizationParameter(BpVisualizationParams parameter, float newScale)
{
	m_visualizationManager.SetVisualizationParameter(parameter, newScale);
}

float BpScene::GetVisualizationParameter(BpVisualizationParams parameter) const
{
	return m_visualizationManager.GetVisualizationParameter(parameter);
}

const BpDebugRenderBuffer& BpScene::GetRenderBuffer() const
{
	return m_visualizationManager.GetDebugRenderBuffer();
}