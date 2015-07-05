#pragma once

#include "BpGeometrySphere.h"
#include "BpGeometryBox.h"
#include "BpGeometryCapsule.h"
#include "../Debugging/BpDebugRenderBuffer.h"
#include "../Actors/BpRigidActor.h"


namespace bPhysics
{
	class BpContactEdge;

	class BpShape
	{
	public:
		BpShape(const BpShape& other);

		BpShape(const BpGeometry& geometry);

		BpMat4x4 GetWorldTransform() const;

		inline void SetOwner(const BpRigidActor* owner) { m_pOwner = owner; }

		inline const BpMat4x4& GetLocalPose() const { return m_localPose; }
		inline void SetLocalPose(const BpMat4x4& pose) { m_localPose = pose; }

		inline const BpGeometryType::Type GetGeometryType() const { return m_geometryHolder.GetGeometry().GetType(); }

		bool GetSphereGeometry(BpGeometrySphere& sphere) const;
		bool GetBoxGeometry(BpGeometryBox& box) const;
		bool GetCapsuleGeometry(BpGeometryCapsule& box) const;
		
		void DebugVisualize(BpDebugRenderBuffer& outBuffer, const BpRigidActor& owner) const;

		std::vector<BpContactEdge*> GetContacts();

	private:
		BpMat4x4 m_localPose;
		BpGeometryHolder m_geometryHolder;
		const BpRigidActor* m_pOwner;

		std::vector<BpContactEdge*> m_contacts;
	};
}