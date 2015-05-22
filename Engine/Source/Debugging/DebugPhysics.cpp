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
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	bool result;

	device = g_pApp->GetGraphicsManager()->GetRenderer()->GetDevice();
	deviceContext = g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext();

	g_pApp->GetGraphicsManager()->GetRenderer()->VGetViewMatrix(m_baseViewMatrix);

	m_pDebugShader = BE_NEW DebugShader;
	if (!m_pDebugShader)
	{
		return false;
	}

	result = m_pDebugShader->Initialize(device);
	if (!result)
	{
		BE_ERROR("Could not initialize the Debug Shader Object");
		return false;
	}

	return true;
}

bool DebugPhysics::Render()
{
	bool result;
	ID3D11DeviceContext* deviceContext = g_pApp->GetGraphicsManager()->GetRenderer()->GetDeviceContext();

	Mat4x4 worldMatrix, orthoMatrix;

	g_pApp->GetGraphicsManager()->GetRenderer()->VGetWorldMatrix(worldMatrix);
	g_pApp->GetGraphicsManager()->GetRenderer()->VGetOrthoMatrix(orthoMatrix);

	IDebugPhysicsRenderBuffer* buffer = g_pApp->m_pGame->VGetGamePhysics()->VGetDebugRenderBuffer();

	std::vector<IDebugPhysicsSphere*> spheres = buffer->VGetDebugSpheres();

	for (int i = 0; i < spheres.size(); i++)
	{
		result = RenderSphere(deviceContext, spheres[i], worldMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}
	}

	return true;
}