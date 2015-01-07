#pragma once

#include "../BombastEngineStd.h"

class ModelClass
{
public:
	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

public:
	ModelClass();
	~ModelClass();

	void SetVertexData(Vec3* vertexData, int count) { m_vertexData = vertexData; m_vertexCount = count; }
	void SetTextureData(Vec3* textureData, int count) { m_textureData = textureData; m_textureCount = count; }
	void SetNormalData(Vec3* normalData, int count) { m_normalData = normalData; m_normalCount = count; }
	void SetFaceData(FaceType* faceData, int count) { m_faceData = faceData; m_faceCount = count; }

	Vec3* GetVertexData() { return m_vertexData; }
	Vec3* GetTextureData() { return m_textureData; }
	Vec3* GetNormalData() { return m_normalData; }
	void GetFaceData(FaceType** faces, int& count) { *faces = m_faceData; count = m_faceCount; }

private:
	Vec3* m_vertexData, *m_textureData, *m_normalData;
	FaceType* m_faceData;

	int m_vertexCount, m_textureCount, m_normalCount, m_faceCount;
};