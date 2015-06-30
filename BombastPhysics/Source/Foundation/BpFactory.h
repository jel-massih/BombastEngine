#pragma once

#include "BpMath.h"
#include "../BpPhysicsCore.h"
#include "../Geometry/BpGeometry.h"

namespace bPhysics
{
	class BpRigidDynamic;

	BpRigidDynamic* BpCreateDynamic(BpPhysicsCore& core, const BpMat4x4& transform, const BpShape& shape, float density);
	BpRigidDynamic* BpCreateDynamic(BpPhysicsCore& core, const BpMat4x4& transform, const BpMat4x4& relativeTransform, const BpGeometry& geometry, BpMaterial& material, float density);
}