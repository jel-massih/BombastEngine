#include "BMDLMeshLoader.h"
#include <vector>
#include "../Graphics3D/ModelClass.h"

void LoadMesh(
	ModelClass** model,
	byte* data)
{
	unsigned int numVertices = *reinterpret_cast<unsigned int*>(data);
	unsigned int numIndices = *reinterpret_cast<unsigned int*>(data + sizeof(unsigned int));

	ModelClass::BasicVertex* vertices = reinterpret_cast<ModelClass::BasicVertex*>(data + (sizeof(unsigned int) * 2));
	unsigned short* indices = reinterpret_cast<unsigned short*>(data + (sizeof(unsigned int) * 2) + (sizeof(ModelClass::BasicVertex) * numVertices));

	(*model)->vertices = std::vector<ModelClass::BasicVertex>(&vertices[0], &vertices[numVertices]);
	(*model)->indices = std::vector<unsigned short>(&indices[0], &indices[numIndices]);
}
