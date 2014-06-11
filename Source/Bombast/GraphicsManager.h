#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "../Graphics3D/D3DClass.h"
#include "../Graphics3D/CameraClass.h"
#include "../Graphics2D/ColorShaderClass.h"
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
	CameraClass* m_pCamera;
	ColorShaderClass* m_pColorShader;
	TextureShaderClass* m_pTextureShader;
};

#endif