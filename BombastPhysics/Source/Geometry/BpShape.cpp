#include "BpShape.h"
#include "../Scene/BpScene.h"
#include "../Debugging/BpVisualizationParams.h"

using namespace bPhysics;

bool BpShape::GetSphereGeometry(BpGeometrySphere& box) const
{
	if (GetGeometryType() != BpGeometryType::SPHERE)
	{
		return false;
	}

	box = static_cast<const BpGeometrySphere&>(m_geometry);
	return true;
}

bool BpShape::GetBoxGeometry(BpGeometryBox& box) const
{
	if (GetGeometryType() != BpGeometryType::BOX)
	{
		return false;
	}

	box = static_cast<const BpGeometryBox&>(m_geometry);
	return true;
}

void BpShape::DebugVisualize(BpDebugRenderBuffer& outBuffer, const BpRigidActor& owner) const
{
	BpScene* scene = owner.GetScene();
	BP_ASSERT(scene);

	float scale = scene->GetVisualizationParameter(BpVisualizationParams::BASE_SCALE);
	if (!scale) { return; }

	if (scene->GetVisualizationParameter(BpVisualizationParams::COLLISION_SHAPES))
	{
		const BpGeometryType::Type type = GetGeometryType();

		switch (type)
		{
		case BpGeometryType::SPHERE:
		{
			BpGeometrySphere sphere;
			GetSphereGeometry(sphere);
			outBuffer << BpDebugSphere(owner.GetWorldTransform().GetPosition(), sphere.radius);
			break;
		}
		case BpGeometryType::BOX:
		{
			BpGeometryBox box;
			GetBoxGeometry(box);
			outBuffer << BpDebugBox(owner.GetWorldTransform().GetPosition(), BpVec3(box.eX, box.eY, box.eZ));
			break;
		}
		default:
			break;
		}
	}
}