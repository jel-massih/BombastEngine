#include "PhysXCharacterController.h"

#include <characterkinematic/PxCapsuleController.h>
#include <characterkinematic/PxControllerManager.h>
#include <PxRigidDynamic.h>

using namespace physx;

PhysXCharacterControllerDesc::PhysXCharacterControllerDesc() :
	mPosition(PxExtendedVec3(0,0,0)),
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

PhysXCharacterController::PhysXCharacterController() :
	m_pController(nullptr)
{
}

PhysXCharacterController::~PhysXCharacterController()
{
	m_pController->release();
}

PxController* PhysXCharacterController::Initialize(const PhysXCharacterControllerDesc& desc, PxControllerManager* manager)
{
	PxCapsuleControllerDesc controllerDesc;

	controllerDesc.height = desc.mHeight;
	controllerDesc.radius = desc.mRadius;
	controllerDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	controllerDesc.density = desc.mProxyDensity;
	controllerDesc.scaleCoeff = desc.mProxyScale;
	controllerDesc.position = desc.mPosition;
	controllerDesc.slopeLimit = desc.mSlopeLimit;
	controllerDesc.stepOffset = desc.mStepOffset;
	controllerDesc.invisibleWallHeight = desc.mInvisibleWallHeight;
	controllerDesc.maxJumpHeight = desc.mMaxJumpHeight;
	controllerDesc.reportCallback = desc.mReportCallback;
	controllerDesc.behaviorCallback = desc.mBehaviorCallback;
	controllerDesc.volumeGrowth = desc.mVolumeGrowth;
	controllerDesc.material = desc.mMaterial;

	PxController* controller = manager->createController(controllerDesc);
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
	return controller;
}