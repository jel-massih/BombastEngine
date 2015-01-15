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

Material::~Material()
{
	for (auto it = m_material.Textures.begin(); it != m_material.Textures.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	m_material.Textures.clear();
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

bool Material::AddTexture(std::string textureName)
{
	TextureClass* texture = BE_NEW TextureClass;
	if (!texture)
	{
		return false;
	}

	if (!texture->Initialize(g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice(), textureName))
	{
		return false;
	}

	m_material.Textures.push_back(texture);

	return true;
}

bool Material::RemoveTexture(std::string textureName)
{
	bool bRemoved = false;
	for (auto it = m_material.Textures.begin(); it != m_material.Textures.end(); it++)
	{
		if ((*it)->GetTextureName() == textureName)
		{
			(*it)->Shutdown();
			it = m_material.Textures.erase(it);
			bRemoved = true;
		}
	}

	return bRemoved;
}