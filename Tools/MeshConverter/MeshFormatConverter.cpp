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
	// Parse arguments

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

	ifstream objFile(argv[1], ifstream::in);
	if (!objFile.is_open())
	{
		cout << "error: could not open file \"" << argv[1] << "\" for read" << endl;
		cin >> x;
		return -1;
	}


	// Parse the object file

	vector<Vector3<float>> positions;
	vector<Vector3<float>> normals;
	vector<Vector2<float>> texcoords;
	vector<vector<IndexTriplet>> faces;

	unsigned int lineNum = 1;

	while (objFile.good())
	{
		string line;
		getline(objFile, line);
		istringstream lineStream(line);

		// Parse the line if not a comment
		if (lineStream.peek() != '#')
		{
			string tag;
			lineStream >> tag;

			// Warn on unsupported tags
			if (
				tag.compare("mtllib") == 0 ||
				tag.compare("o") == 0 ||
				tag.compare("g") == 0 ||
				tag.compare("usemtl") == 0 ||
				tag.compare("s") == 0
				)
			{
				cout << argv[1] << "(" << lineNum << "): warning: BasicMesh VBO format does not support tag \"" << tag << "\"" << endl;
			}

			// Parse the vertex position
			else if (tag.compare("v") == 0)
			{
				Vector3<float> pos;
				lineStream >> pos.x >> pos.y >> pos.z;
				positions.push_back(pos);
			}

			// Parse the vertex normal
			else if (tag.compare("vn") == 0)
			{
				Vector3<float> norm;
				lineStream >> norm.x >> norm.y >> norm.z;
				normals.push_back(norm);
			}

			// Parse the vertex texture coordinate
			else if (tag.compare("vt") == 0)
			{
				Vector2<float> tex;
				lineStream >> tex.x >> tex.y;
				texcoords.push_back(tex);
			}

			// Parse the face
			else if (tag.compare("f") == 0)
			{
				vector<IndexTriplet> face;
				while (lineStream.good())
				{
					string tripletString;
					lineStream >> tripletString;
					if (tripletString.size() > 0)
					{
						istringstream tripletStream(tripletString);

						IndexTriplet triplet;
						triplet.pos = 0;
						triplet.norm = 0;
						triplet.tex = 0;

						// Parse the face triplet.  Valid formats include:
						//  [pos] ...
						//  [pos]/[tex] ...
						//  [pos]/[tex]/[norm] ...
						//  [pos]// [norm] ...

						tripletStream >> triplet.pos;
						if (tripletStream.get() == '/')
						{
							if (tripletStream.peek() != '/')
							{
								tripletStream >> triplet.tex;
							}
							if (tripletStream.get() == '/')
							{
								tripletStream >> triplet.norm;
							}
						}
						face.push_back(triplet);
					}
				}
				faces.push_back(face);
			}

			// Fail on unknown tag
			else if (tag.size() > 0)
			{
				cout << argv[1] << "(" << lineNum << "): error: unknown tag \"" << tag << "\"" << endl;
			}
		}
		lineNum++;
	}

	objFile.close();

	if (positions.size() == 0 || faces.size() == 0)
	{
		cout << "error: obj file \"" << argv[1] << "\" contains no geometry" << endl;
		cin >> x;
		return -1;
	}

	// Validate mesh data

	for (auto face = faces.begin(); face != faces.end(); face++)
	{
		if (face->size() < 3)
		{
			cout << "error: face size " << face->size() << " invalid" << endl;
			cin >> x;
			return -1;
		}
		for (auto triplet = face->begin(); triplet != face->end(); triplet++)
		{
			if (triplet->pos > positions.size() || triplet->pos < 1)
			{
				cout << "error: position index " << triplet->pos << " out of range" << endl;
				cin >> x;
				return -1;
			}
			if (triplet->norm > normals.size() || triplet->norm < 0)
			{
				cout << "error: normal index " << triplet->norm << " out of range" << endl;
				cin >> x;
				return -1;
			}
			if (triplet->tex > texcoords.size() || triplet->tex < 0)
			{
				cout << "error: texcoord index " << triplet->tex << " out of range" << endl;
				cin >> x;
				return -1;
			}
		}
	}

	// Get the bounding box and center of the mesh

	Vector3<float> boxMin = positions[faces[0][0].pos - 1];
	Vector3<float> boxMax = boxMin;
	for (auto face = faces.begin(); face != faces.end(); face++)
	{
		for (auto triplet = face->begin(); triplet != face->end(); triplet++)
		{
			Vector3<float> pos = positions[triplet->pos - 1];
			boxMin.x = min(boxMin.x, pos.x);
			boxMin.y = min(boxMin.y, pos.y);
			boxMin.z = min(boxMin.z, pos.z);
			boxMax.x = max(boxMax.x, pos.x);
			boxMax.y = max(boxMax.y, pos.y);
			boxMax.z = max(boxMax.z, pos.z);
		}
	}
	Vector3<float> boxCenter = (boxMax + boxMin) / 2.0f;

	// If specified in the arguments, normalize geometry to fit within a unit cube

	if (normalizePositions)
	{
		float maxAxis = max(max(boxMax.x - boxMin.x, boxMax.y - boxMin.y), boxMax.z - boxMin.z);
		for (auto pos = positions.begin(); pos != positions.end(); pos++)
		{
			*pos = (*pos - boxCenter) / maxAxis;
		}
	}

	// Generate missing normals using faceted technique

	for (auto face = faces.begin(); face != faces.end(); face++)
	{
		Vector3<float> normal(0, 0, 0);
		bool normalGenerated = false;
		for (auto triplet = face->begin(); triplet != face->end(); triplet++)
		{
			if (!normalGenerated && triplet->norm == 0)
			{
				for (auto triplet = face->begin(); triplet != face->end(); triplet++)
				{
					Vector3<float> posThis = positions[triplet->pos - 1];
					Vector3<float> posPrev = positions[(triplet == face->begin() ? (face->end() - 1)->pos : (triplet - 1)->pos) - 1];
					Vector3<float> posNext = positions[(triplet == face->end() - 1 ? (face->begin())->pos : (triplet + 1)->pos) - 1];
					normal = normal + cross(posNext - posThis, posPrev - posThis);
					triplet->norm = normals.size() + 1;
				}
				normals.push_back(normal);
				normalGenerated = true;
			}
		}
	}

	// Fill in missing texture coordinates with (0, 0)

	bool missingTexcoordCreated = false;
	unsigned int missingTexcoordIndex = 0;
	for (auto face = faces.begin(); face != faces.end(); face++)
	{
		for (auto triplet = face->begin(); triplet != face->end(); triplet++)
		{
			if (triplet->tex == 0)
			{
				if (!missingTexcoordCreated)
				{
					texcoords.push_back(Vector2<float>(0.0f, 0.0f));
					missingTexcoordIndex = texcoords.size();
					missingTexcoordCreated = true;
				}
				triplet->tex = missingTexcoordIndex;
			}
		}
	}

	// Generate unique vertices and convert counter-clockwise faces to clockwise triangles

	vector<BasicVertex> vertices;
	vector<unsigned short> indices;
	map<IndexTriplet, unsigned short> tripletIndices;
	for (auto face = faces.begin(); face != faces.end(); face++)
	{
		for (auto triplet = face->begin(); triplet != face->end(); triplet++)
		{
			if (tripletIndices.find(*triplet) == tripletIndices.end())
			{
				tripletIndices[*triplet] = static_cast<unsigned short>(vertices.size());
				BasicVertex vertex;
				vertex.pos = positions[triplet->pos - 1];
				vertex.norm = normals[triplet->norm - 1];
				vertex.tex = texcoords[triplet->tex - 1];
				vertices.push_back(vertex);
			}
			if (triplet >= face->begin() + 2)
			{
				indices.push_back(tripletIndices[*face->begin()]);
				indices.push_back(tripletIndices[*(triplet - 1)]);
				indices.push_back(tripletIndices[*triplet]);
			}
		}
	}

	// Dump vertex and index data to the output VBO file

	ofstream vboFile(argv[2], ofstream::out | ofstream::binary);
	if (!vboFile.is_open())
	{
		cout << "error: could not open file \"" << argv[2] << "\" for write" << endl;
		cin >> x;
		return -1;
	}

	unsigned int numVertices = vertices.size();
	unsigned int numIndices = indices.size();
	vboFile.write(reinterpret_cast<char*>(&numVertices), sizeof(unsigned int));
	vboFile.write(reinterpret_cast<char*>(&numIndices), sizeof(unsigned int));
	vboFile.write(reinterpret_cast<char*>(&vertices[0]), sizeof(BasicVertex) * vertices.size());
	vboFile.write(reinterpret_cast<char*>(&indices[0]), sizeof(unsigned short) * indices.size());

	vboFile.close();

	return 0;
}