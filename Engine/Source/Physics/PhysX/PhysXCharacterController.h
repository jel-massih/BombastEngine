#pragma once
#include <characterkinematic/PxExtended.h>

namespace physx
{
	class PxController;
	class PxUserControllerHitReport;
	class PxControllerBehaviorCallback;
	class PxControllerManager;
	class PxPhysics;
	class PxScene;
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

class PhysXCharacterController
{
public:
	PhysXCharacterController();
	~PhysXCharacterController();

	PxController* Initialize(const PhysXCharacterControllerDesc& desc, PxControllerManager* manager);

protected:
	PxController* m_pController;

};