#include "BpShape.h"
#include "../Scene/BpScene.h"
#include "../Debugging/BpVisualizationParams.h"

using namespace bPhysics;

BpShape::BpShape(const BpShape& other) : m_transform(other.m_transform)
{
	m_geometryHolder.SetGeometry(other.m_geometryHolder.GetGeometry());
}

BpShape::BpShape(const BpGeometry& geometry)
{
	m_geometryHolder.SetGeometry(geometry);
}

bool BpShape::GetSphereGeometry(BpGeometrySphere& sphere) const
{
	if (GetGeometryType() != BpGeometryType::SPHERE)
	{
		return false;
	}

	sphere = static_cast<const BpGeometrySphere&>(m_geometryHolder.GetGeometry());
	return true;
}

bool BpShape::GetBoxGeometry(BpGeometryBox& box) const
{
	if (GetGeometryType() != BpGeometryType::BOX)
	{
		return false;
	}

	box = static_cast<const BpGeometryBox&>(m_geometryHolder.GetGeometry());
	return true;
}

bool BpShape::GetCapsuleGeometry(BpGeometryCapsule& box) const
{
	if (GetGeometryType() != BpGeometryType::CAPSULE)
	{
		return false;
	}

	box = static_cast<const BpGeometryCapsule&>(m_geometryHolder.GetGeometry());
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
		case BpGeometryType::CAPSULE:
		{
			BpGeometryCapsule capsule;
			GetCapsuleGeometry(capsule);
			outBuffer << BpDebugCapsule(owner.GetWorldTransform().GetPosition(), capsule.radius, capsule.halfHeight);
			break;
		}
		default:
			break;
		}
	}
}