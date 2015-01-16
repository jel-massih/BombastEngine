#pragma once

#include <d3d9types.h>
#include "../Graphics2D/TextureClass.h"

class Material
{
public:
	Material();
	~Material();

	void SetAmbient(const Vec4& color);
	const Vec4 GetAmbient() const { return m_material.Ambient; }

	void SetDiffuse(const Vec4& color);
	const Vec4 GetDiffuse() const { return m_material.Diffuse; }

	void SetSpecular(const Vec4& color, const float power);
	void GetSpecular(Vec4 &_color, float &_power) const { _color = m_material.Specular; _power = m_material.Power; }

	void SetEmissive(const Vec4& color);
	const Vec4 GetEmissive() const { return m_material.Emissive; }

	void SetAlpha(const float alpha);
	bool HasAlpha() const { return GetAlpha() != fOPAQUE; }
	float GetAlpha() const { return m_material.Diffuse.w; }

	bool AddTexture(std::string textureName);
	bool RemoveTexture(std::string textureName);

	const std::vector<TextureClass*>& GetTextures() const { return m_material.Textures; }

	ShaderType GetShaderType() const { return m_material.ShaderType; }

	void SetShaderType(std::string shaderName);
	void SetShaderType(ShaderType shaderType);

	XMFLOAT2 GetUVScale() const { return m_material.UVScale; }
	void SetUVScale(XMFLOAT2 scale);

private:
	struct BEMaterial
	{
		Vec4 Ambient;
		Vec4 Diffuse;
		Vec4 Specular;
		float Power;
		Vec4 Emissive;
		std::vector<TextureClass*> Textures;
		ShaderType ShaderType;
		XMFLOAT2 UVScale;
	};
	BEMaterial m_material;
};