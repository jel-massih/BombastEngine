#pragma once

#include "BpGeometrySphere.h"
#include "BpGeometryBox.h"
#include "BpGeometryCapsule.h"
#include "../Debugging/BpDebugRenderBuffer.h"
#include "../Actors/BpRigidActor.h"


namespace bPhysics
{
	class BpShape
	{
	public:
		BpShape(const BpShape& other);

		BpShape(const BpGeometry& geometry);

		inline const BpMat4x4& GetWorldTransform() const { return m_transform; }
		inline void SetWorldTransform(const BpMat4x4& t) { m_transform = t; }

		inline const BpGeometryType::Type GetGeometryType() const { return m_geometryHolder.GetGeometry().GetType(); }

		bool GetSphereGeometry(BpGeometrySphere& sphere) const;
		bool GetBoxGeometry(BpGeometryBox& box) const;
		bool GetCapsuleGeometry(BpGeometryCapsule& box) const;
		
		void DebugVisualize(BpDebugRenderBuffer& outBuffer, const BpRigidActor& owner) const;
	private:
		BpMat4x4 m_transform;
		BpGeometryHolder m_geometryHolder;
	};
}