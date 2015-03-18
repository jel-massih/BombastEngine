#pragma once

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

		virtual BpScene* GetScene() const = 0;

		virtual void SetName(const char* name) = 0;
		virtual const char* GetName() const = 0;



	protected:
		BpActor() {}
		virtual ~BpActor() {}
	};
}