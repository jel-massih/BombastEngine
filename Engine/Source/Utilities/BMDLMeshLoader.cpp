#include "BMDLMeshLoader.h"
#include <vector>
#include "../Graphics3D/ModelClass.h"

void LoadMesh(
	ModelClass** model,
	byte* data)
{
	const auto szui = sizeof(unsigned int);

	int offset = 0;
	unsigned int numMeshes = *reinterpret_cast<unsigned int*>(data);
	offset += szui;

	for (unsigned int i = 0; i < numMeshes; i++)
	{
		unsigned int materialIndex = *reinterpret_cast<unsigned int*>(data + offset);
		offset += szui;

		unsigned int numVertices = *reinterpret_cast<unsigned int*>(data + offset);
		offset += szui;
		unsigned int numIndices = *reinterpret_cast<unsigned int*>(data + offset);
		offset += szui;

		ModelClass::SubMesh mesh;
		ModelClass::BasicVertex* vertices = reinterpret_cast<ModelClass::BasicVertex*>(data + offset);
		offset += sizeof(ModelClass::BasicVertex) * numVertices;
		unsigned int* indices = reinterpret_cast<unsigned int*>(data + offset);
		offset += sizeof(unsigned int) * numIndices;

		mesh.vertices = std::vector<ModelClass::BasicVertex>(&vertices[0], &vertices[numVertices]);
		mesh.indices = std::vector<unsigned int>(&indices[0], &indices[numIndices]);
		
		mesh.materialIndex = materialIndex;

		(*model)->meshes.push_back(mesh);
	}
}