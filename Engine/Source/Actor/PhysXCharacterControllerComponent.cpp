#include "PhysXCharacterControllerComponent.h"
#include "TransformComponent.h"

#include "../Physics/PhysXPhysics.h"

#include <characterkinematic/PxCapsuleController.h>
#include <characterkinematic/PxControllerManager.h>
#include <PxRigidDynamic.h>

using namespace physx;

PhysXCharacterControllerDesc::PhysXCharacterControllerDesc() :
	mPosition(PxExtendedVec3(0, 0, 0)),
	mSlopeLimit(0.0f),
	mContactOffset(0.0f),
	mStepOffset(0.0f),
	mInvisibleWallHeight(0.0f),
	mMaxJumpHeight(0.0f),
	mRadius(0.0f),
	mHeight(0.0f),
	mCrouchHeight(0.0f),
	mProxyDensity(10.0f),
	mProxyScale(0.9f),
	mVolumeGrowth(1.5f),
	mMaterial(NULL),
	mReportCallback(NULL),
	mBehaviorCallback(NULL)
{}

const char* PhysXCharacterControllerComponent::g_Name = "PhysXCharacterControllerComponent";

PhysXCharacterControllerComponent::PhysXCharacterControllerComponent()
	: m_pController(nullptr), m_pPhysXPhysics(nullptr)
{
}

PhysXCharacterControllerComponent::~PhysXCharacterControllerComponent()
{
	m_pController->release();
}


bool PhysXCharacterControllerComponent::VInitialize(rapidxml::xml_node<>* pData)
{
	return true;
}

void PhysXCharacterControllerComponent::VPostInit()
{
	PxCapsuleControllerDesc controllerDesc;

	controllerDesc.height = m_controllerDesc.mHeight;
	controllerDesc.radius = m_controllerDesc.mRadius;
	controllerDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	controllerDesc.density = m_controllerDesc.mProxyDensity;
	controllerDesc.scaleCoeff = m_controllerDesc.mProxyScale;
	controllerDesc.position = m_controllerDesc.mPosition;
	controllerDesc.slopeLimit = m_controllerDesc.mSlopeLimit;
	controllerDesc.stepOffset = m_controllerDesc.mStepOffset;
	controllerDesc.invisibleWallHeight = m_controllerDesc.mInvisibleWallHeight;
	controllerDesc.maxJumpHeight = m_controllerDesc.mMaxJumpHeight;
	controllerDesc.reportCallback = m_controllerDesc.mReportCallback;
	controllerDesc.behaviorCallback = m_controllerDesc.mBehaviorCallback;
	controllerDesc.volumeGrowth = m_controllerDesc.mVolumeGrowth;
	controllerDesc.material = m_controllerDesc.mMaterial;

	m_pPhysXPhysics = static_cast<PhysXPhysics*>(g_pApp->m_pGame->VGetGamePhysics());

	PxController* controller = m_pPhysXPhysics->GetPxControllerManager()->createController(controllerDesc);
	PxRigidDynamic* controllerActor = controller->getActor();
	if (controllerActor != nullptr)
	{
		if (controllerActor->getNbShapes() > 0)
		{
			PxShape* controllerShape;
			controllerActor->getShapes(&controllerShape, 1);
			controllerShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
		}
	}

	m_pController = controller;
}

void PhysXCharacterControllerComponent::VUpdate(const float deltaMs)
{
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name);

	if (!pTransformComponent)
	{
		BE_ERROR("No Transform Component!");
		return;
	}

	PxExtendedVec3 pos = m_pController->getPosition();
	Vec3 bPos;
	PhysXPhysicsHelpers::PxExtendedVecToVec3(pos, &bPos);

	pTransformComponent->SetPosition(bPos);
}

void PhysXCharacterControllerComponent::onShapeHit(const PxControllerShapeHit& hit)
{
}

PxControllerBehaviorFlags PhysXCharacterControllerComponent::getBehaviorFlags(const PxShape& shape, const PxActor& actor)
{
	return PxControllerBehaviorFlags(0);
}

PxControllerBehaviorFlags PhysXCharacterControllerComponent::getBehaviorFlags(const PxController&)
{
	return PxControllerBehaviorFlags(0);
}

PxControllerBehaviorFlags PhysXCharacterControllerComponent::getBehaviorFlags(const PxObstacle&)
{
	return PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT | PxControllerBehaviorFlag::eCCT_SLIDE;
}