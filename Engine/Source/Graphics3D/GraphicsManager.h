#pragma once

#include "../Graphics3D/D3DClass.h"
#include "../Shading/ShaderManager.h"

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

	const ShaderManager* GetShaderManager() const {
		return m_pShaderManager;
	}

private:
	IRenderer* m_pRenderer;
	ShaderManager* m_pShaderManager;
};