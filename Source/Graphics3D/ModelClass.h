#pragma once

#include "../BombastEngineStd.h"
#include "../Utilities/TinyObjLoader/tiny_obj_loader.h"

class ModelClass
{
public:
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
};