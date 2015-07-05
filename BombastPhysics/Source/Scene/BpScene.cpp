#include "../msvc/stdafx.h"
#include "BpScene.h"
#include "../Actors/BpRigidDynamic.h"
#include "../Constraints/BpIsland.h"
#include "../Constraints/BpContact.h"
#include <stack>

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

		//Add Behind Check for debug?
		//Add to visualization
		body->SetupBodyDebugging(m_visualizationManager);
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

//1. Apply Forces
//2. Check Existing Contacts
//3. Find New COntacts
//4. Resolve Collisions via impulses
//5. Satisfy Constaraints
void BpScene::Simulate(float timestep)
{
	for (auto it = m_sceneRigidDynamics.begin(); it != m_sceneRigidDynamics.end(); it++)
	{
		//Clear island Bit
		(*it)->SetInIsland(false);

		if (!(*it)->IsSleeping())
		{
			(*it)->Simulate(timestep);
		}
	}

	m_contactManager.ClearContactIslandFlags();

	BuildIslands();
}

void BpScene::BuildIslands()
{
	//Make Island Worst Case Scenario
	BpIsland island;

	//Build Islands & solve
	for (auto it = m_sceneRigidDynamics.begin(); it != m_sceneRigidDynamics.end(); it++)
	{
		BpRigidDynamic* seed = (*it);
		//Must be in Island
		if (seed->IsInIsland()) {
			continue;
		}

		//Must not be sleeping
		if ((*it)->IsSleeping()) {
			continue;
		}

		seed->SetInIsland(true);

		std::stack<BpRigidDynamic*> targetBodies;
		targetBodies.push(*it);

		//perform DFS on constraint graph
		while (!targetBodies.empty())
		{
			BpRigidDynamic* body = targetBodies.top();
			targetBodies.pop();

			island.Add(body->GetShape());

			body->WakeUp();

			for (auto contactIt = body->GetShape()->GetContacts().begin(); it != body->GetShape()->GetContacts().end; it++)
			{
				BpContactConstraint* contact = (*contactIt)->constraint;

				if (contact->bInIsland || !contact->bColliding) {
					continue;
				}

				contact->bInIsland = true;
				island.Add(contact);

				BpRigidDynamic* other = (*contactIt)->other;
				if (other->IsInIsland()) {
					continue;
				}

				other->SetInIsland(true);
				targetBodies.push(other);
			}
		}

		assert(island.m_shapes.size() != 0);

		island.Initialize();
		island.Solve();
	}

	//TODO: Update broadphase AABB

	//Find New Contacts
	m_contactManager.FindContacts();

	//TODO: CLear FOrces
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