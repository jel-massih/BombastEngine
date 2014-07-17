#ifndef MATERIAL_H
#define MATERIAL_H

#include <d3d9types.h>

class Material
{
	D3DMATERIAL9 m_D3DMat;
public:
	Material();

	void SetAmbient(const Color& color);
	const Color GetAmbient() { return m_D3DMat.Ambient; }

	void SetDiffuse(const Color& color);
	const Color GetDiffuse() { return m_D3DMat.Diffuse; }

	void SetSpecular(const Color& color, const float power);
	void GetSpecular(Color &_color, float &_power) { _color = m_D3DMat.Specular; _power = m_D3DMat.Power; }

	void SetEmissive(const Color& color);
	const Color GetEmissive() { return m_D3DMat.Emissive; }

	void SetAlpha(const float alpha);
	bool HasAlpha() const { return GetAlpha() != fOPAQUE; }
	float GetAlpha() const { return m_D3DMat.Diffuse.a; }
};

#endif