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
		DirectX::XMFLOAT2 relativeSize,
		RenderPass renderPass,
		const Mat4x4 *t);

	virtual HRESULT VOnRestore(Scene* pScene) = 0;
	virtual HRESULT VOnLostDevice(Scene* pScene) { return S_OK; }
	virtual HRESULT VRender(Scene* pScene) = 0;

protected:
	virtual HRESULT InitializeBuffers() = 0;
	HRESULT LoadTexture(std::string filename);

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

protected:
	std::string m_textureName;

	TextureShaderClass m_shader;

	int m_bitmapWidth;
	int m_bitmapHeight;

	DirectX::XMFLOAT2 m_relativeSize;

	int m_vertexCount; //Number of Vertices in Vertex Array
	int m_indexCount; //Number of Indices in Index array

	ID3D11Buffer *m_pVertexBuffer, *m_pIndexBuffer;

	ID3D11ShaderResourceView* m_pTexture;
};

class D3DBitmapNode11 : public BitmapNode
{
public:
	D3DBitmapNode11(const ActorId actorId, BaseRenderComponent* renderComponent, std::string textureFileName, DirectX::XMFLOAT2 relativeSize, RenderPass renderPass, const Mat4x4* t);
	HRESULT VOnRestore(Scene* pScene);
	HRESULT VRender(Scene* pScene);

private:
	HRESULT InitializeBuffers();

protected:
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pVertexBuffer;
};

#endif