#pragma once

class DebugShader;

class DebugPhysics
{
public:
	DebugPhysics();
	~DebugPhysics();

	bool Initialize();
	bool Render();

private:
	DebugShader* m_pDebugShader;
	Mat4x4 m_baseViewMatrix;
};