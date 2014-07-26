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

#endif