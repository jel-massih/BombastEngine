#ifndef BITMAP_NODE_H
#define BITMAP_NODE_H

#include "Material.h"
#include "SceneNode.h"

class BitmapNode : public SceneNode
{
public:
	BitmapNode(const char* textureFile);
	virtual ~BitmapNode();
	HRESULT VPreRender(Scene* pScene);

protected:
	const char* m_textureName;

	std::string GetTextureName(const int side);
};

class D3DBitmapNode11 : public BitmapNode
{
public:
	D3DBitmapNode11(const char* pTextureBaseNode);
	virtual ~D3DBitmapNode11();
	HRESULT VOnRestore(Scene* pScene);
	HRESULT VRender(Scene* pScene);

protected:
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pVertexBuffer;
};

#endif