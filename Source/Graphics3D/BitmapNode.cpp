#include "BitmapNode.h"
#include "../Bombast/BombastApp.h"

BitmapNode::BitmapNode(const ActorId actorId,
	BaseRenderComponent* renderComponent,
	std::string textureFileName,
	RenderPass renderPass,
	const Mat4x4 *t) : SceneNode(actorId, renderComponent, renderPass, t)
{
	m_textureName = textureFileName;
}

D3DBitmapNode11::D3DBitmapNode11(const ActorId actorId,
	BaseRenderComponent* renderComponent,
	std::string textureFileName,
	RenderPass renderPass,
	const Mat4x4* t) : BitmapNode(actorId, renderComponent, textureFileName, renderPass, t)
{}

HRESULT D3DBitmapNode11::VOnRestore(Scene* pScene)
{
	HRESULT hr;
	BE_HRETURN(SceneNode::VOnRestore(pScene), "Failed to OnRestore Parent");

	//Reload the Texture
	{
		Resource resource(m_textureName);
		ResourceHandle* pResHandle = g_pApp->m_pResourceCache->GetHandle(&resource);
		TextureResourceExtraData* extra = static_cast<TextureResourceExtraData*>(pResHandle->GetExtra());
	}
	return S_OK;
}

HRESULT D3DBitmapNode11::VRender(Scene* pScene)
{
	HRESULT hr;

	//Get Texture
	Resource resource(m_textureName);
	ResourceHandle* pResourceHandle = g_pApp->m_pResourceCache->GetHandle(&resource);
	TextureResourceExtraData* extra = static_cast<TextureResourceExtraData*>(pResourceHandle->GetExtra());

	//Render

	return S_OK;
}