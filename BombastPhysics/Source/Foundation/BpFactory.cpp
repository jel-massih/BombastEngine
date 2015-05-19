#include "../msvc/stdafx.h"
#include "BpFactory.h"
#include "BpReporting.h"
#include "../Actors/BpRigidDynamic.h"

namespace bPhysics 
{

	bool isDynamicGeometry(BpGeometryType::Type type)
	{
		return type == BpGeometryType::SPHERE;
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

	BpRigidDynamic* BpCreateDynamic(BpPhysicsCore& core, const BpMat4x4& transform, const BpGeometry& geometry, BpMaterial& material, float density)
	{
		if (!transform.IsValid())
		{
			BP_ERROR("BpCreateDynamic: Transform is not Valid");
		}

		if (!isDynamicGeometry(geometry.GetType()))
		{
			return nullptr;
		}

		BpShape* shape = core.CreateShape(geometry, material, true);
		if (!shape)
		{
			return nullptr;
		}

		BpRigidDynamic* body = BpCreateDynamic(core, transform, *shape, density);

		BP_SAFE_DELETE(shape);

		return body;
	}

}