#ifndef BITMAP_NODE_H
#define BITMAP_NODE_H

#include "Material.h"
#include "SceneNode.h"
#include "../Graphics2D/TextureShaderClass.h"

class BitmapNode : public SceneNode
{
public:
	BitmapNode(const ActorId actorId,
		BaseRenderComponent* renderComponent,
		std::string textureFileName,
		RenderPass renderPass,
		const Mat4x4 *t);

	virtual HRESULT VOnRestore(Scene* pScene);
	virtual HRESULT VOnLostDevice(Scene* pScene) { return S_OK; }
	virtual HRESULT VRender(Scene* pScene);

protected:
	std::string m_textureName;

	TextureShaderClass m_shader;
};

class D3DBitmapNode11 : public BitmapNode
{
public:
	D3DBitmapNode11(const ActorId actorId, BaseRenderComponent* renderComponent, RenderPass renderPass, const Mat4x4* t);
	virtual ~D3DBitmapNode11();
	HRESULT VOnRestore(Scene* pScene);
	HRESULT VRender(Scene* pScene);

protected:
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pVertexBuffer;
};

#endif