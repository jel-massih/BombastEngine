#include "Material.h"

Material::Material()
{
	Vec4 ambientMat = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ZeroMemory(&m_material, sizeof(D3DMATERIAL9));
	m_material.Diffuse = g_White;
	m_material.Ambient = ambientMat;
	m_material.Specular = g_White;
	m_material.Emissive = g_Black;
	m_material.Power = 32.0f;
}

void Material::SetAmbient(const Vec4& color)
{
	m_material.Ambient = color;
}

void Material::SetDiffuse(const Vec4& color)
{
	m_material.Diffuse = color;
}

void Material::SetSpecular(const Vec4& color, const float power)
{
	m_material.Specular = color;
	m_material.Power = power;
}

void Material::SetEmissive(const Vec4& color)
{
	m_material.Emissive = color;
}

void Material::SetAlpha(const float alpha)
{
	m_material.Diffuse.w = alpha;
}