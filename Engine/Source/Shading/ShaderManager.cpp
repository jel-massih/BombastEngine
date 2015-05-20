#include "ShaderManager.h"

ShaderManager::~ShaderManager()
{
	SAFE_DELETE(m_pStaticMeshShader);
}

bool ShaderManager::Initialize(IRenderer* renderer)
{
	bool result;

	m_pStaticMeshShader = BE_NEW StaticMeshShader;
	if (!m_pStaticMeshShader)
	{
		BE_ERROR("Could not Allocate the StaticMeshShader Object!");
		return false;
	}

	result = m_pStaticMeshShader->Initialize(renderer->GetDevice());
	if (!result)
	{
		BE_ERROR("Could not initialize the StaticMeshShader Object!");
		return false;
	}

	return true;
}

bool ShaderManager::RenderStaticMesh(SceneNode* pRenderableNode, Material* pMaterial, int indexCount, const Scene* pScene) const
{
	ID3D11DeviceContext* context = g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext();
	return m_pStaticMeshShader->Render(context, indexCount, pMaterial, pScene);
}