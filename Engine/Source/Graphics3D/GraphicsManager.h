#pragma once

#include "../Graphics3D/D3DClass.h"
#include "../Graphics2D/BasicShaders.h"
#include "../Graphics2D/LightShader.h"

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

	LightShader* GetLightShader() const {
		return m_pLightShader;
	}

	ColorShader* GetColorShader() const {
		return m_pColorShader;
	}

	TextureShader* GetTextureShader() const {
		return m_pTextureShader;
	}

private:
	IRenderer* m_pRenderer;
	LightShader* m_pLightShader;
	ColorShader* m_pColorShader;
	TextureShader* m_pTextureShader;
};