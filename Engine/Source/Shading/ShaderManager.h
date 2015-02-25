#pragma once

#include "BasicShaders.h"
#include "StaticMeshShader.h"
#include "MultiTextureShader.h"
#include "LightmapShader.h"

class SceneNode;

class ShaderManager
{

public:
	ShaderManager() : m_pStaticMeshShader(nullptr){}
	~ShaderManager();

	bool Initialize(IRenderer* renderer);

	bool RenderStaticMesh(SceneNode* pRenderableNode, Material* pMaterial, int indexCount, const Scene* pScene) const;

private:
	StaticMeshShader* m_pStaticMeshShader;
};