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


public:
	std::vector<BasicVertex> vertices;
	std::vector<unsigned short> indices;
};