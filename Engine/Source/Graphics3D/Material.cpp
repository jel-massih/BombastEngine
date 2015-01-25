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
	m_material.UVScale = XMFLOAT2(1.0f, 1.0f);
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
			it = m_material.Textures.erase(it);
			bRemoved = true;
		}
	}

	return bRemoved;
}

void Material::SetShaderType(std::string shaderName)
{
	m_material.ShaderType = BSHADER_TYPE_INVALID;

	if (shaderName == "Color")
	{
		m_material.ShaderType = BSHADER_TYPE_COLOR;
	}
	if (shaderName == "Font")
	{
		m_material.ShaderType = BSHADER_TYPE_FONT;
	}
	if (shaderName == "LitTextured")
	{
		m_material.ShaderType = BSHADER_TYPE_LIT;
	}
	if (shaderName == "Texture")
	{
		m_material.ShaderType = BSHADER_TYPE_TEXTURE;
	}
	if (shaderName == "MultiTexture")
	{
		m_material.ShaderType = BSHADER_TYPE_MULTI_TEXTURE;
	}
	if (shaderName == "Lightmap")
	{
		m_material.ShaderType = BSHADER_TYPE_LIGHTMAP;
	}
	if (shaderName == "DeferredLit")
	{
		m_material.ShaderType = BSHADER_TYPE_DEFERRED_LIT;
	}
}

void Material::SetShaderType(ShaderType shaderType)
{
	m_material.ShaderType = shaderType;
}

void Material::SetUVScale(XMFLOAT2 scale)
{
	m_material.UVScale = scale;
}