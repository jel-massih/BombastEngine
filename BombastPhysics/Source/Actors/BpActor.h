#pragma once

#include "../Geometry/BpBounds3.h"

namespace bPhysics
{
	class BpScene;

	struct BpActorType
	{
		enum Type
		{
			RIGID_DYNAMIC
		};
	};

	class BpActor
	{
	public:
		virtual BpActorType::Type GetType() const = 0;

		virtual void SetName(const char* name) = 0;
		virtual const char* GetName() const = 0;

		inline virtual BpScene* GetScene() const = 0;

		virtual BpBounds3 GetWorldBounds(float inflation = 1.01f) const = 0;

	protected:
		BpActor() {}
		virtual ~BpActor() {}
	};
}