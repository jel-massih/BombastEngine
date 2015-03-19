#pragma once

#include "BpGeometry.h"
#include "BpGeometrySphere.h"
#include "../Actors/BpRigidActor.h"

namespace bPhysics
{
	class BpShape
	{
	public:
		virtual BpGeometryType::Type GetGeometryType() const = 0;
		virtual void SetGeometry(const BpGeometry& geometry) = 0;

		virtual bool GetSphereGeometry(BpGeometrySphere& geometry) const = 0;

		virtual BpRigidActor* GetActor() const = 0;

		virtual void SetLocalPose(const BpMat4x4& pose) = 0;
		virtual BpMat4x4 GetLocalPose() const = 0;

	protected:
		inline BpShape() {}
		virtual ~BpShape() {}
	};
}