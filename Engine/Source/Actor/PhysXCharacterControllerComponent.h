#pragma once
#include "ActorComponent.h"

#include <characterkinematic/PxExtended.h>
#include <characterkinematic/PxControllerBehavior.h>
#include <characterkinematic/PxController.h>

class PhysXPhysics;

namespace physx
{
	class PxControllerManager;
}

using namespace physx;

struct PhysXCharacterControllerDesc
{
	PhysXCharacterControllerDesc();

	PxExtendedVec3					mPosition;
	float							mSlopeLimit;
	float							mContactOffset;
	float							mStepOffset;
	float							mInvisibleWallHeight;
	float							mMaxJumpHeight;
	float							mRadius;
	float							mHeight;
	float							mCrouchHeight;
	float							mProxyDensity;
	float							mProxyScale;
	float							mVolumeGrowth;
	PxMaterial*						mMaterial;
	PxUserControllerHitReport*		mReportCallback;
	PxControllerBehaviorCallback*	mBehaviorCallback;
};

class PhysXCharacterControllerComponent : public ActorComponent, public PxUserControllerHitReport, public PxControllerBehaviorCallback
{
public:
	const static char* g_Name;
	virtual const char* VGetName() const override { return PhysXCharacterControllerComponent::g_Name; }

	PhysXCharacterControllerComponent();
	~PhysXCharacterControllerComponent();

	virtual bool VInitialize(rapidxml::xml_node<>* pData) override;
	virtual void VPostInit() override;
	virtual void VUpdate(const float deltaMs) override;
	virtual rapidxml::xml_node<>* VGenerateXml(rapidxml::xml_document<> &outDoc) override { return NULL; }


	// Implements PxUserControllerHitReport
	virtual void							onShapeHit(const PxControllerShapeHit& hit);
	virtual void							onControllerHit(const PxControllersHit& hit) {}
	virtual void							onObstacleHit(const PxControllerObstacleHit& hit) {}

	// Implements PxControllerBehaviorCallback
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxShape& shape, const PxActor& actor);
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxController& controller);
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxObstacle& obstacle);

protected:
	PxController* m_pController;

	PhysXPhysics* m_pPhysXPhysics;

	PhysXCharacterControllerDesc m_controllerDesc;

	char* m_materialName;
};