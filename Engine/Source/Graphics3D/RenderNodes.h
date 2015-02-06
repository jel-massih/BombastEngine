#pragma once

#include "SceneNode.h"
#include "ModelClass.h"

//Forawrd Declares
class ModelClass;

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
	virtual HRESULT VForwardRender(Scene* pScene) = 0;

protected:
	virtual HRESULT InitializeBuffers() = 0;
	virtual HRESULT LoadTexture(std::string filename) = 0;
	bool VIsVisible(Scene* pScene) const { return true; }

	int GetIndexCount() const {
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
	HRESULT VForwardRender(Scene* pScene);

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

class D3D11GridNode : public SceneNode
{
private:
	struct VertexType
	{
		Vec3 position;
		Vec4 color;
	};

public:
	D3D11GridNode(ActorId actorId, BaseRenderComponent* renderComponent, const Mat4x4* pMatrix);
	~D3D11GridNode();

	HRESULT VOnRestore(Scene* pScene);
	HRESULT VForwardRender(Scene* pScene);

	virtual bool VIsVisible(Scene* pScene) const { return m_bShow; }

	void SetVisible(bool bVisible) { m_bShow = bVisible; }

private:
	HRESULT InitializeBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

protected:
	int m_vertCount, m_indexCount;
	int m_gridWidth, m_gridHeight;

	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pVertexBuffer;

	Material* m_material;

	bool m_bShow;
};

class MeshNode : public SceneNode
{
public:

	MeshNode(const ActorId actorId,
		BaseRenderComponent* renderComponent,
		std::string meshFileName,
		RenderPass renderPass,
		const Mat4x4 *t);

	virtual HRESULT VOnRestore(Scene* pScene) = 0;
	virtual HRESULT VOnLostDevice(Scene* pScene) { return S_OK; }
	virtual HRESULT VDeferredRender(Scene* pScene) = 0;

protected:
	virtual HRESULT InitializeBuffers() = 0;
	virtual HRESULT InitializeSubMeshBuffers(const ModelClass::SubMesh& submesh, ID3D11Buffer** pVertexBuffer, ID3D11Buffer** pIndexBuffer) = 0;

	virtual Material* LoadMaterial(std::string filename) = 0;

	bool VIsVisible(Scene* pScene) const { return true; }

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

protected:
	std::string m_meshFilename;
};

class D3DMeshNode11 : public MeshNode
{
public:
	D3DMeshNode11(const ActorId actorId, BaseRenderComponent* renderComponent, std::string meshFilename, std::vector<std::string> materialsFilename, RenderPass renderPass, const Mat4x4* t);
	~D3DMeshNode11();

	HRESULT VOnRestore(Scene* pScene);
	HRESULT VDeferredRender(Scene* pScene);
	HRESULT VForwardRender(Scene* pScene);

private:
	HRESULT InitializeBuffers();
	HRESULT InitializeSubMeshBuffers(const ModelClass::SubMesh& submesh, ID3D11Buffer** pVertexBuffer, ID3D11Buffer** pIndexBuffer);
	Material* LoadMaterial(std::string filename);
	HRESULT LoadMesh(std::string meshFilename);
	HRESULT UpdateBuffers(ID3D11DeviceContext* deviceContext);
	bool RenderDeferredBuffers(ID3D11DeviceContext* deviceContext);
	bool RenderForwardBuffers(ID3D11DeviceContext* deviceContext, Scene* pScene);

private:
	struct SubMeshBuffers
	{
		ID3D11Buffer *pVertexBuffer, *pIndexBuffer;
		unsigned int vertexCount, indexCount;
		Material* material;
	};

	ModelClass* m_pLoadedMesh;
	Vec3 m_lastPosition;
	std::vector<SubMeshBuffers> m_submeshBuffers;
	std::vector<std::string> m_materialFilenames;
};