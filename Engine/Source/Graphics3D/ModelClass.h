#pragma once

class ModelClass
{
public:
	struct BasicVertex
	{
		struct {
			float x;
			float y;
			float z;
		} pos;

		struct {
			float u;
			float v;
		} tex;

		struct {
			float x;
			float y;
			float z;
		} normal;
	};

	struct SubMesh
	{
		std::vector<BasicVertex> vertices;
		std::vector<unsigned int> indices;

		unsigned int materialIndex;
	};

public:

	std::vector<SubMesh> meshes;
};