#include "BatchedRenderables.h"

int BatchedRenderables::AddVertex(const Vec3& pos, const Vec2& texCoord, const Vec3& color)
{
	int vertIndex = m_batchVertices.size();
	m_batchVertices.push_back(SimpleBatchedVertex(pos, texCoord));
	return vertIndex;
}

void BatchedRenderables::DeclareTriangle(int vert0, int vert1, int vert2)
{
	m_batchIndices.push_back(vert0);
	m_batchIndices.push_back(vert1);
	m_batchIndices.push_back(vert2);
}

void BatchedRenderables::AllocateIndexArray(int arraySize)
{
	m_batchIndices.reserve(arraySize);
}

void BatchedRenderables::ExpandIndexArray(int additionalIndexCount)
{
	m_batchIndices.reserve(m_batchIndices.size() + additionalIndexCount);
}

void BatchedRenderables::AllocateVertexArray(int arraySize)
{
	m_batchVertices.reserve(arraySize);
}

void BatchedRenderables::ExpandVertexArray(int additionalVertexCount)
{
	m_batchVertices.reserve(m_batchVertices.size() + additionalVertexCount);
}

void BatchedRenderables::ClearArrays()
{
	m_batchIndices.clear();
	m_batchVertices.clear();
}

bool BatchedRenderables::Render()
{
	if (m_batchIndices.size() <= 0 || m_batchVertices.size() <= 0)
	{
		return true;
	}


}