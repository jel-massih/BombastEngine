//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

#include "BasicMath.h"
#include "tiny_obj_loader.h"

using namespace std;

struct BasicVertex
{
	Vector3<float> pos;  // position
	Vector2<float> tex;  // texture coordinate
	Vector3<float> norm; // surface normal vector
};

struct IndexTriplet
{
	unsigned short pos;
	unsigned short norm;
	unsigned short tex;
};

bool operator <(const IndexTriplet& lhs, const IndexTriplet& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(IndexTriplet)) < 0;
}

void usage()
{
	cout << "usage: obj2vbo.exe [input obj] [output vbo] [/normalize]" << endl;
	cout << "  input obj    specifies the input OBJ mesh file" << endl;
	cout << "  output vbo   specifies the output VBO file" << endl;
	cout << "  /normalize   transform vertex positions to fit a unit bounding box about the origin" << endl;
}

#pragma warning(disable : 4447)

int main(int argc, char *argv[])
{
	int x;

	if (argc < 3 || argc > 4)
	{
		usage();
		cin >> x;
		return -1;
	}

	bool normalizePositions = false;
	if (argc == 4)
	{
		if (strcmp(argv[3], "/normalize") == 0)
		{
			normalizePositions = true;
		}
		else
		{
			usage();
			cin >> x;
			return -1;
		}
	}

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	
	std::string err = tinyobj::LoadObj(shapes, materials, argv[1]);

	if (!err.empty())
	{
		std::cerr << err << endl;
		cin >> x;
		return -1;
	}

	cout << shapes.size() << " Shapes found" << endl;
	
	const auto sz = sizeof(unsigned long);
	
	// Dump vertex and index data to the output VBO file
	ofstream vboFile(argv[2], ofstream::out | ofstream::binary);
	if (!vboFile.is_open())
	{
		cerr << "error: could not open file \"" << argv[2] << "\" for write" << endl;
		cin >> x;
		return -1;
	}


	unsigned long numObjects = shapes.size();

	vboFile.write(reinterpret_cast<char*>(&numObjects), sz);

	for (int i = 0; i < numObjects; i++)
	{
		unsigned long numVertices = shapes[i].mesh.positions.size();
		unsigned long numNormals = shapes[i].mesh.normals.size();
		unsigned long numTexCoords = shapes[i].mesh.texcoords.size();
		unsigned long numIndices = shapes[i].mesh.indices.size();
		unsigned long numMaterials = shapes[i].mesh.material_ids.size();

		vboFile.write(reinterpret_cast<char*>(&numVertices), sz);
		vboFile.write(reinterpret_cast<char*>(&numNormals), sz);
		vboFile.write(reinterpret_cast<char*>(&numTexCoords), sz);
		vboFile.write(reinterpret_cast<char*>(&numIndices), sz);
		vboFile.write(reinterpret_cast<char*>(&numMaterials), sz);

		vboFile.write(reinterpret_cast<char*>(&shapes[i].mesh.positions[0]), numVertices * sizeof(float));
		if (numNormals > 0) {
			vboFile.write(reinterpret_cast<char*>(&shapes[i].mesh.normals[0]), numNormals * sizeof(float));
		}
		if (numTexCoords > 0) {
			vboFile.write(reinterpret_cast<char*>(&shapes[i].mesh.texcoords[0]), numTexCoords * sizeof(float));
		}
		vboFile.write(reinterpret_cast<char*>(&shapes[i].mesh.indices[0]), numIndices * sizeof(unsigned int));
		vboFile.write(reinterpret_cast<char*>(&shapes[i].mesh.material_ids[0]), numMaterials * sizeof(float));
	}

	vboFile.close();

	//Convert mtl to set of material files
	cout << materials.size() << " Materials found" << endl;

	for (auto it = materials.begin(); it != materials.end(); it++)
	{
		tinyobj::material_t& mat = (*it);
		ofstream mtlFile(mat.name + ".bmtl", ofstream::out | ofstream::binary);
		if (!mtlFile.is_open())
		{
			cerr << "error: could not open file \"" << mat.name << ".bmtl" << "\" for write" << endl;
			cin >> x;
			return -1;
		}

		mtlFile << "<Material name=\"" << mat.name << "\">" << endl;
		mtlFile << "\t<Ambient r=\"" << mat.ambient[0] << "\" g=\"" << mat.ambient[1] << "\" b=\"" << mat.ambient[2] << "\" a=\"1\"/>" << endl;
		mtlFile << "\t<Diffuse r=\"" << mat.diffuse[0] << "\" g=\"" << mat.diffuse[1] << "\" b=\"" << mat.diffuse[2] << "\" a=\"1\"/>" << endl;
		mtlFile << "\t<Specular r=\"" << mat.specular[0] << "\" g=\"" << mat.specular[1] << "\" b=\"" << mat.specular[2] << "\" a=\"1\" power=\"" << mat.shininess << "\" />" << endl;
		mtlFile << "\t<Emmisive r=\"" << mat.emission[0] << "\" g=\"" << mat.emission[1] << "\" b=\"" << mat.emission[2] << "\" a=\"1\"/>" << endl;
		
		mtlFile << "\t<Textures>" << endl;
		mtlFile << "\t\t<Texture path=\"" << mat.diffuse_texname << ".dds\"/>";
		mtlFile << "\t</Textures>" << endl;

		mtlFile << "\t<Shader type=\"LitTextured\"/>" << endl;
		mtlFile << "</Material>";

		mtlFile.close();
	}

	return 0;
}