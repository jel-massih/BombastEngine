#include "Material.h"

Material::Material()
{
	ZeroMemory(&m_D3DMat, sizeof(D3DMATERIAL9));
	m_D3DMat.Diffuse = g_White;
	m_D3DMat.Ambient = { 0.10f, 0.10f, 0.10f, 1.0f };
	m_D3DMat.Specular = g_White;
	m_D3DMat.Emissive = g_Black;
}

void Material::SetAmbient(const Color& color)
{
	m_D3DMat.Ambient = color;
}

void Material::SetDiffuse(const Color& color)
{
	m_D3DMat.Diffuse = color;
}

void Material::SetSpecular(const Color& color, const float power)
{
	m_D3DMat.Specular = color;
	m_D3DMat.Power = power;
}

void Material::SetEmissive(const Color& color)
{
	m_D3DMat.Emissive = color;
}

void Material::SetAlpha(const float alpha)
{
	m_D3DMat.Diffuse.a = alpha;
}