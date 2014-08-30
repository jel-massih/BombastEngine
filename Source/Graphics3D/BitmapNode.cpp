#include "BitmapNode.h"

BitmapNode::BitmapNode(const char* pTextureBaseName) : 
	SceneNode(INVALID_ACTOR_ID, NULL, RenderPass_Sky, &Mat4x4::g_Identity)
{
	m_textureName = pTextureBaseName;
}

BitmapNode::~BitmapNode()
{
}

HRESULT BitmapNode::VPreRender(Scene* pScene)
{
	return SceneNode::VPreRender(pScene);
}