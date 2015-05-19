#include "DebugPhysics.h"
#include "../Shading/DebugShader.h"

DebugPhysics::DebugPhysics()
{
	m_pDebugShader = 0;
}

DebugPhysics::~DebugPhysics()
{
	SAFE_DELETE(m_pDebugShader);
}

bool DebugPhysics::Initialize()
{
	return true;
}

bool DebugPhysics::Render()
{
	return true;
}