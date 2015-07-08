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
	
	BpMat4x4 translation;
	translation.BuildTranslation(t.GetPosition() + m_localPose.GetPosition());

	BpMat4x4 rotation;
	BpVec3 rot1 = t.GetYawPitchRoll();
	BpVec3 rot2 = m_localPose.GetYawPitchRoll();
	rotation.BuildYawPitchRoll(rot1.x + rot2.x, rot1.y + rot2.y, rot1.z + rot2.z);

	BpMat4x4 scale;
	scale.BuildScale(m_localPose.GetScale() * t.GetScale());

	return scale * rotation * translation;
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

	f32 scale = scene->GetVisualizationParameter(BpVisualizationParams::BASE_SCALE);
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

std::vector<BpContactEdge*> BpShape::GetContacts()
{
	return m_contacts;
}