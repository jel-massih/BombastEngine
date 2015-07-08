#pragma once

#include "BpActor.h"
#include "../Foundation/BpMat.h"

namespace bPhysics
{
	class BpRigidActor : public BpActor
	{
	public:
		//Retrieves actors world space transform
		virtual BpMat4x4 GetWorldTransform() const = 0;
		virtual void SetWorldTransform(const BpMat4x4& newTransform, bool autoWake = true) = 0;
	
	protected:
		inline BpRigidActor() {}
		virtual ~BpRigidActor() {}
	};
}