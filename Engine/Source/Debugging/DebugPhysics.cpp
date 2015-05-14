#include "DebugPhysics.h"

DebugPhysics::DebugPhysics()
{
	m_pDebugShader = 0;
}

DebugPhysics::~DebugPhysics()
{
	SAFE_DELETE(m_pDebugShader);
}

bool DebugPh