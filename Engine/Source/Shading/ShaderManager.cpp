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
}

bool ShaderManager::RenderRenderable(SceneNode* pRenderableNode, Material* pMaterial, int indexCount, const Scene* pScene) const
{
	IRenderer* pRenderer = g_pApp->GetGraphicsManager()->GetRenderer();
	ID3D11DeviceContext* context = pRenderer->GetDeviceContext();

	Mat4x4 worldMatrix, viewMatrix, projectionMatrix;
	pRenderer->VGetViewMatrix(viewMatrix);
	pRenderer->VGetWorldMatrix(worldMatrix);
	pRenderer->VGetProjectionMatrix(projectionMatrix);

	switch (pMaterial->GetShaderType())
	{
	case BSHADER_TYPE_LIT:
		return m_pStaticMeshShader->Render(context, indexCount, pMaterial, pScene);
		break;
	}

	return false;
}