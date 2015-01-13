#pragma once

#include <d3d9types.h>

class Material
{
	struct BEMaterial
	{
		Vec4 Ambient;
		Vec4 Diffuse;
		Vec4 Specular;
		float Power;
		Vec4 Emissive;
	};
	BEMaterial m_material;
public:
	Material();

	void SetAmbient(const Vec4& color);
	const Vec4 GetAmbient() { return m_material.Ambient; }

	void SetDiffuse(const Vec4& color);
	const Vec4 GetDiffuse() { return m_material.Diffuse; }

	void SetSpecular(const Vec4& color, const float power);
	void GetSpecular(Vec4 &_color, float &_power) { _color = m_material.Specular; _power = m_material.Power; }

	void SetEmissive(const Vec4& color);
	const Vec4 GetEmissive() { return m_material.Emissive; }

	void SetAlpha(const float alpha);
	bool HasAlpha() const { return GetAlpha() != fOPAQUE; }
	float GetAlpha() const { return m_material.Diffuse.w; }
};