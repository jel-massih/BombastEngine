#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "../Graphics3D/D3DClass.h"
#include "../Graphics2D/BitmapClass.h"
#include "../Graphics2D/TextureShaderClass.h"

class GraphicsManager
{
public:
	GraphicsManager();
	GraphicsManager(const GraphicsManager& other);
	~GraphicsManager();

	bool Initialize(HWND hwnd);
	void Shutdown();
	bool Frame();
private:
	bool Render();

private:
	D3DClass* m_pD3D;
	BitmapClass* m_pBitmap;
	TextureShaderClass* m_pTextureShader;
};

#endif