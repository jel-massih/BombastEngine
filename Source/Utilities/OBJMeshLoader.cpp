#include "OBJMeshLoader.h"
#include <sstream>

#include "../Graphics3D/ModelClass.h"

namespace OBJMeshLoader
{
	bool ReadFileCounts(char* pRawBuffer, int& vertCount, int& texCount, int& normalCount, int& faceCount);
	bool LoadDataStructures(char* pRawBuffer, int vertCount, int texCount, int normalCount, int faceCount, ModelClass* model);

	bool LoadModel(char* pRawBuffer, ModelClass* model)
	{
		bool result;
		int vertCount, texCount, normalCount, faceCount;

		result = ReadFileCounts(pRawBuffer, vertCount, texCount, normalCount, faceCount);
		if (!result)
		{
			return false;
		}

		result = LoadDataStructures(pRawBuffer, vertCount, texCount, normalCount, faceCount, model);
		if (!result)
		{
			return false;
		}

		return true;
	}

	bool OBJMeshLoader::ReadFileCounts(char* pRawBuffer, int& vertCount, int& texCount, int& normalCount, int& faceCount)
	{
		char input;

		vertCount = 0;
		texCount = 0;
		normalCount = 0;
		faceCount = 0;

		std::stringstream ss(pRawBuffer);

		if (ss.fail())
		{
			return false;
		}

		ss.get(input);
		while (!ss.eof())
		{
			if (input == 'v')
			{
				ss.get(input);
				if (input == ' ') { vertCount++; }
				if (input == 't') { texCount++; }
				if (input == 'n') { normalCount++; }
			}

			if (input == 'f')
			{
				ss.get(input);
				if (input == ' ') { faceCount++; }
			}

			while (input != '\n')
			{
				ss.get(input);
			}

			ss.get(input);
		}

		return true;
	}

	bool LoadDataStructures(char* pRawBuffer, int vertCount, int texCount, int normalCount, int faceCount, ModelClass* model)
	{
		Vec3* vertices, *texCoords, *normals;
		ModelClass::FaceType* faces;
		int vertexIndex, texIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
		char input, input2;

		std::stringstream ss(pRawBuffer);

		if (ss.fail())
		{
			return false;
		}

		vertices = BE_NEW Vec3[vertCount];
		if (!vertices)
		{
			return false;
		}

		texCoords = BE_NEW Vec3[texCount];
		if (!texCoords)
		{
			return false;
		}

		normals = BE_NEW Vec3[normalCount];
		if (!normals)
		{
			return false;
		}

		faces = BE_NEW ModelClass::FaceType[faceCount];
		if (!faces)
		{
			return false;
		}

		vertexIndex = 0;
		texIndex = 0;
		normalIndex = 0;
		faceIndex = 0;

		ss.get(input);
		while (!ss.eof())
		{
			if (input == 'v')
			{
				ss.get(input);

				if (input == ' ')
				{
					ss >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;
					//Invert to convert to LH System
					vertices[vertexIndex].z *= -1.0f;
					vertexIndex++;
				}

				if (input == 't')
				{
					ss >> texCoords[texIndex].x >> texCoords[texIndex].y;
					//Invert to convert to LH System
					texCoords[texIndex].y = 1.0f - texCoords[texIndex].y;
					texIndex++;
				}

				if (input == 'n')
				{
					ss >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

					//Invert to convert to LH System
					normals[normalIndex].z *= -1.0f;
					normalIndex++;
				}
			}

			if (input == 'f')
			{
				ss.get(input);
				if (input == ' ')
				{
					ss >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3;
					ss >> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2;
					ss >> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
					faceIndex++;
				}
			}

			while (input != '\n')
			{
				ss.get(input);
			}

			ss.get(input);
		}

		BE_INFO(ToStr(vertCount));

		model->SetVertexData(vertices, vertCount);
		model->SetTextureData(texCoords, texCount);
		model->SetNormalData(normals, normalCount);
		model->SetFaceData(faces, faceCount);

		return true;
	}
}