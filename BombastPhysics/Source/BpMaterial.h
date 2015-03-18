#pragma once

namespace bPhysics
{
	class BpMaterial
	{
	public:
		virtual void Release() = 0;

		virtual void SetDynamicFriction(float coefficient) = 0;
		virtual float GetDynamicFriction() const = 0;

		virtual void SetStaticFriction(float coefficient) = 0;
		virtual float GetStaticFriction() const = 0;

		virtual void SetRestitution(float restitution) = 0;
		virtual float GetResititution() const = 0;

		virtual const char* GetTypeName() const { return "BpMaterial"; }

	protected:
		inline BpMaterial() {}
		virtual ~BpMaterial();
	};
}