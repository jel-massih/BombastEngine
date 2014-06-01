#include "CameraClass.h"

CameraClass::CameraClass()
{
	m_posX = 0.0f;
	m_posY = 0.0f;
	m_posZ = 0.0f;

	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_rotZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotX = x;
	m_rotY = y;
	m_rotZ = z;
}

DirectX::XMFLOAT3 CameraClass::GetPosition()
{
	return DirectX::XMFLOAT3(m_posX, m_posY, m_posZ);
}

DirectX::XMFLOAT3 CameraClass::GetRotation()
{
	return DirectX::XMFLOAT3(m_rotX, m_rotY, m_rotZ);
}

void CameraClass::Render()
{
	DirectX::XMVECTOR up, pos, lookAt;
	float yaw, pitch, roll;
	DirectX::XMMATRIX rotationMatrix;

	up = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	pos = DirectX::XMVectorSet(m_posX, m_posX, m_posZ, 0.0f);
	lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	//Set badd boys to radians
	pitch = m_rotX * 0.0174532925f;
	yaw = m_rotY * 0.0174532925f;
	roll = m_rotZ * 0.0174532925f;

	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAt = DirectX::XMVector3TransformCoord(lookAt, rotationMatrix);
	up = DirectX::XMVector3TransformCoord(up, rotationMatrix);
	
	lookAt = DirectX::XMVectorAdd(pos, lookAt);

	m_viewMatrix = DirectX::XMMatrixLookAtLH(pos, lookAt, up);
}

void CameraClass::GetViewMatrix(DirectX::XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}