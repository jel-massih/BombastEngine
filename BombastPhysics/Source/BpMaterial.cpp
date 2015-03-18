#include "BpMaterial.h"

using namespace bPhysics;

void BpMaterial::SetDynamicFriction(float coefficient)
{
	m_materialData.dynamicFriction = coefficient;
}

float BpMaterial::GetDynamicFriction() const
{
	return m_materialData.dynamicFriction;
}

void BpMaterial::SetStaticFriction(float coefficient)
{
	m_materialData.staticFriction = coefficient;
}

float BpMaterial::GetStaticFriction() const
{
	return m_materialData.staticFriction;
}

void BpMaterial::SetRestitution(float restitution)
{
	m_materialData.restitution = restitution;
}

float BpMaterial::GetResititution() const
{
	return m_materialData.restitution;
}