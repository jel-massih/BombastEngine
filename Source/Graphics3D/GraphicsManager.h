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
	IRenderer* GetRenderer();

	enum Renderer
	{
		Renderer_Unknown,
		Renderer_D3D9,
		Renderer_D3D11
	};

	static Renderer GetRendererImpl();

	TextureShaderClass* GetTextureShader() const {
		return m_pTextureShader;
	}

private:
	IRenderer* m_pRenderer;
	ColorShaderClass* m_pColorShader;
	TextureShaderClass* m_pTextureShader;
};

#endif