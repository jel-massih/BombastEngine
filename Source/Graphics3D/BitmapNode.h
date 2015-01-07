#pragma once

#include "SceneNode.h"

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
	virtual HRESULT LoadTexture(std::string filename) = 0;
	bool VIsVisible(Scene* pScene) const;

	int GetIndexCount() {
		return m_indexCount;
	}

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

protected:
	std::string m_textureName;

	int m_bitmapWidth;
	int m_bitmapHeight;

	DirectX::XMFLOAT2 m_relativeSize;

	int m_vertexCount; //Number of Vertices in Vertex Array
	int m_indexCount; //Number of Indices in Index array
};

class D3DBitmapNode11 : public BitmapNode
{
public:
	D3DBitmapNode11(const ActorId actorId, BaseRenderComponent* renderComponent, std::string textureFileName, DirectX::XMFLOAT2 relativeSize, RenderPass renderPass, const Mat4x4* t);
	HRESULT VOnRestore(Scene* pScene);
	HRESULT VRender(Scene* pScene);

private:
	HRESULT InitializeBuffers();
	HRESULT LoadTexture(std::string filename);
	HRESULT UpdateBuffers(ID3D11DeviceContext* deviceContext);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer *m_pVertexBuffer, *m_pIndexBuffer;
	ID3D11ShaderResourceView* m_pTexture;
	Vec3 m_lastPosition;
	
};