#pragma once

class ModelClass;

namespace OBJMeshLoader
{
	bool LoadModel(char* pRawBuffer, ModelClass* model);
};