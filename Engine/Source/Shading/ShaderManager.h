#pragma once

#include "BasicShaders.h"
#include "LightShader.h"
#include "MultiTextureShader.h"
#include "LightmapShader.h"

class SceneNode;

class ShaderManager
{

public:
	ShaderManager() : m_pLightShader(nullptr), m_pColorShader(nullptr), m_pTextureShader(nullptr), m_pMultiTextureShader(nullptr) {}
	~ShaderManager();

	bool Initialize(IRenderer* renderer);

	bool RenderRenderable(SceneNode* pRenderableNode, Material* pMaterial, int indexCount, const Scene* pScene) const;

private:
	LightShader* m_pLightShader;
	ColorShader* m_pColorShader;
	TextureShader* m_pTextureShader;
	MultiTextureShader* m_pMultiTextureShader;
	LightmapShader* m_pLightmapShader;
};