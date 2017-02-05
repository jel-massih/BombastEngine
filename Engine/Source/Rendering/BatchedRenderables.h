#pragma once

//BatchedRenderables is used for joining many renderable objects into a single later drawcall (UI Elements)

class BatchedRenderables
{
public:
	BatchedRenderables() {}

	//Add a vertex to batch buffer
	int AddVertex(const Vec3& pos, const Vec2& texCoord, const Vec3& color);
	
	//Adds to index buffer to specify a triangle based off previously added vertices
	void DeclareTriangle(int vert0, int vert1, int vert2);

	//Allocates the index array with the specified size
	void AllocateIndexArray(int arraySize);
	//Expands the index buffer for current size + additional size
	void ExpandIndexArray(int additionalIndexCount);

	//Allocates the vertex array with the specified size
	void AllocateVertexArray(int arraySize);
	//Expands the vertex buffer for current size + additional size
	void ExpandVertexArray(int additionalVertexCount);

	//Clears all batched data
	void ClearArrays();

	//Render this batch
	bool Render();

private:

};