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

	bool RayInTriangle(Vec3 RP1, Vec3 RP2, Vec3 TP1, Vec3 TP2, Vec3 TP3, Vec3& hitPos)
	{
		Vec3 normal, intersectPos;
		Vec3 temp1 = TP2 - TP1;
		Vec3 temp2 = TP3 - TP1;
	
		//Find Triangle Normal
		normal = temp1.Cross(temp2);
		normal.Normalize();

		//Dist from RP1 and RP2 to plane by tri
		float dist1 = (RP1 - TP1).Dot(normal);
		float dist2 = (RP2 - TP1).Dot(normal);
		if ((dist1 * dist2) >= 0.0f) { return false; } //Line does not corss triangle
		if (dist1 == dist2) { return false; } //parallel

		intersectPos = RP1 + (RP2 - RP1) * (-dist1 / (dist2 - dist1));

		Vec3 vtest;
		vtest = normal.Cross(TP2 - TP1);
		if (vtest.Dot(intersectPos - TP1) < 0.0f) { return false; }

		vtest = normal.Cross(TP3 - TP2);
		if (vtest.Dot(intersectPos - TP2) < 0.0f) { return false; }

		vtest = normal.Cross(TP1 - TP3);
		if (vtest.Dot(intersectPos - TP1) < 0.0f) { return false; }

		hitPos = intersectPos;
		return true;
	}
};