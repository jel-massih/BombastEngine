#pragma once

namespace bPhysics
{
	struct BpMaterialData
	{
		float dynamicFriction;
		float staticFriction;
		float restitution;

		BpMaterialData()
			: dynamicFriction(0.0f), staticFriction(0.0f), restitution(0.0f) 
		{}
	};

	class BpMaterial
	{
	public:
		void SetDynamicFriction(float coefficient);
		float GetDynamicFriction() const;

		void SetStaticFriction(float coefficient);
		float GetStaticFriction() const;

		void SetRestitution(float restitution);
		float GetResititution() const;

	private:
		BpMaterialData m_materialData;
	};
}