#include "BpShape.h"
#include "../Scene/BpScene.h"
#include "../Debugging/BpVisualizationParams.h"
#include "../Actors/BpRigidActor.h"

using namespace bPhysics;

BpShape::BpShape(const BpShape& other) : m_localPose(other.m_localPose), m_pOwner(other.m_pOwner)
{
	m_geometryHolder.SetGeometry(other.m_geometryHolder.GetGeometry());
}

BpShape::BpShape(const BpGeometry& geometry)
{
	m_geometryHolder.SetGeometry(geometry);
}

BpMat4x4 BpShape::GetWorldTransform() const
{
	BpMat4x4 t = m_pOwner->GetWorldTransform();
	
	//Pos
	BpVec3 pos = t.GetPosition();
	pos += m_localPose.GetPosition();
	t.SetPosition(pos);

	//scale
	t.SetScale(m_localPose.GetScale() * t.GetScale());
	
	return t;
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
			outBuffer << BpDebugSphere(sphere.radius, this);
			break;
		}
		case BpGeometryType::BOX:
		{
			BpGeometryBox box;
			GetBoxGeometry(box);
			outBuffer << BpDebugBox(BpVec3(box.eX, box.eY, box.eZ), this);
			break;
		}
		case BpGeometryType::CAPSULE:
		{
			BpGeometryCapsule capsule;
			GetCapsuleGeometry(capsule);
			outBuffer << BpDebugCapsule(capsule.radius, capsule.halfHeight, this);
			break;
		}
		default:
			break;
		}
	}
}