#pragma once

#include "../BombastEngineStd.h"
#include <stack>
#include <DirectXMath.h>
#include "../Graphics3D/Geometry.h"

using namespace DirectX;

class BMMatrixStack
{
public:
	BMMatrixStack();

	XMFLOAT4X4* Top() {
		return &m_matrixStack.top();
	}
	void Pop() {
		m_matrixStack.pop();
	}

	void LoadIdentity();
	void LoadMatrix(const XMMATRIX* pM);
	void LoadMatrix(const XMFLOAT4X4* pM) {
		m_matrixStack.pop();
		m_matrixStack.push(*pM);
	}

	//Duplicates matrix at top of stack
	void Push();

	void MultMatrixLocal(const XMMATRIX* pM);
	void MultMatrixLocal(const XMFLOAT4X4* pM);
	void MultMatrix(const XMMATRIX* pM);
	void MultMatrix(const XMFLOAT4X4* pM);

private:
	std::stack<XMFLOAT4X4> m_matrixStack;
};

namespace BEMath
{
	Mat4x4 RotateToFace(Vec3 direction, Vec3 up);
};