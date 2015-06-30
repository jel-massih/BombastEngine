#include "../msvc/stdafx.h"
#include "BpFactory.h"
#include "BpReporting.h"
#include "../Actors/BpRigidDynamic.h"

namespace bPhysics 
{

	bool isDynamicGeometry(BpGeometryType::Type type)
	{
		return type == BpGeometryType::SPHERE || 
			type == BpGeometryType::BOX ||
			type == BpGeometryType::CAPSULE;
	}

	BpRigidDynamic* BpCreateDynamic(BpPhysicsCore& core, const BpMat4x4& transform, const BpShape& shape, float density)
	{
		if (!transform.IsValid())
		{
			BP_ERROR("BpCreateDynamic: Transform is not Valid");
		}

		BpRigidDynamic* body = new BpRigidDynamic(shape);

		body->SetWorldTransform(transform);

		//@TODO: Do something with density

		return body;
	}

	BpRigidDynamic* BpCreateDynamic(BpPhysicsCore& core, const BpMat4x4& transform, const BpMat4x4& relativeTransform, const BpGeometry& geometry, BpMaterial& material, float density)
	{
		if (!transform.IsValid())
		{
			BP_ERROR("BpCreateDynamic: Transform is not Valid");
		}

		if (!relativeTransform.IsValid())
		{
			BP_ERROR("BpCreateDynamic: Realtive Transform is not Valid");
		}

		if (!isDynamicGeometry(geometry.GetType()))
		{
			return nullptr;
		}

		BpShape* shape = core.CreateShape(geometry, material);
		if (!shape)
		{
			return nullptr;
		}

		shape->SetLocalPose(relativeTransform);

		BpRigidDynamic* body = BpCreateDynamic(core, transform, *shape, density);

		BP_SAFE_DELETE(shape);

		return body;
	}

}