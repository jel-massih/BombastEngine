#pragma once

namespace bPhysics
{
	struct BpMaterialData
	{
		f32 dynamicFriction;
		f32 staticFriction;
		f32 restitution;

		BpMaterialData()
			: dynamicFriction(0.0f), staticFriction(0.0f), restitution(0.0f) 
		{}
	};

	class BpMaterial
	{
	public:
		void SetDynamicFriction(f32 coefficient);
		f32 GetDynamicFriction() const;

		void SetStaticFriction(f32 coefficient);
		f32 GetStaticFriction() const;

		void SetRestitution(f32 restitution);
		f32 GetResititution() const;

	private:
		BpMaterialData m_materialData;
	};
}