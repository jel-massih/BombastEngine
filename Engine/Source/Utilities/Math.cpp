#include "Math.h"

BMMatrixStack::BMMatrixStack()
{
	XMFLOAT4X4 matrix;

	XMStoreFloat4x4(&matrix, XMMatrixIdentity());

	m_matrixStack.push(matrix);
}

void BMMatrixStack::LoadIdentity()
{
	XMStoreFloat4x4(&m_matrixStack.top(), XMMatrixIdentity());
}

void BMMatrixStack::LoadMatrix(const XMMATRIX* pM)
{
	XMStoreFloat4x4(&m_matrixStack.top(), *pM);
}

void BMMatrixStack::Push()
{
	XMFLOAT4X4 topMatrix;
	topMatrix = m_matrixStack.top();
	m_matrixStack.push(topMatrix);
}

void BMMatrixStack::MultMatrix(const XMMATRIX* pM)
{
	XMStoreFloat4x4(&m_matrixStack.top(), XMMatrixMultiply(XMLoadFloat4x4(&m_matrixStack.top()), *pM));
}

void BMMatrixStack::MultMatrix(const XMFLOAT4X4* pM)
{
	XMStoreFloat4x4(&m_matrixStack.top(), XMMatrixMultiply(XMLoadFloat4x4(&m_matrixStack.top()), XMLoadFloat4x4(pM)));
}

void BMMatrixStack::MultMatrixLocal(const XMMATRIX* pM)
{
	XMStoreFloat4x4(&m_matrixStack.top(), XMMatrixMultiply(*pM, XMLoadFloat4x4(&m_matrixStack.top())));
}

void BMMatrixStack::MultMatrixLocal(const XMFLOAT4X4* pM)
{
	XMStoreFloat4x4(&m_matrixStack.top(), XMMatrixMultiply(XMLoadFloat4x4(pM), XMLoadFloat4x4(&m_matrixStack.top())));
}

namespace BEMath
{
	Mat4x4 RotateToFace(Vec3 sourcePosition, Vec3 lookAtPosition, Vec3 up)
	{
		Vec3 direction = lookAtPosition - sourcePosition;

		Vec3 Right = up.Cross(direction);
		Right = Right.Normalize();
		Vec3 Back = direction.Normalize();
		Vec3 Up = Back.Cross(Right);

		return Mat4x4(XMFLOAT4X4(Right.x, Right.y, Right.z, 0,
			Up.x, Up.y, Up.z, 0,
			Back.x, Back.y, Back.z, 0,
			0, 0, 0, 1));
	}
};