#include "BpMaterial.h"
#include "Foundation\BpSimpleTypes.h"

using namespace bPhysics;

void BpMaterial::SetDynamicFriction(f32 coefficient)
{
	m_materialData.dynamicFriction = coefficient;
}

f32 BpMaterial::GetDynamicFriction() const
{
	return m_materialData.dynamicFriction;
}

void BpMaterial::SetStaticFriction(f32 coefficient)
{
	m_materialData.staticFriction = coefficient;
}

f32 BpMaterial::GetStaticFriction() const
{
	return m_materialData.staticFriction;
}

void BpMaterial::SetRestitution(f32 restitution)
{
	m_materialData.restitution = restitution;
}

f32 BpMaterial::GetResititution() const
{
	return m_materialData.restitution;
}