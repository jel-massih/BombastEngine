#pragma once

class DebugShader;

class DebugPhysics
{
private:
	struct DebugShapeType
	{
		const char* shapeId;
		ID3D11Buffer* vertexBuffer, *indexBuffer;
		int vertexCount, indexCount;
		float red, green, blue;
		Mat4x4 transformMatrix;
	};

	struct VertexType
	{
		XMFLOAT3 position;
	};

public:
	DebugPhysics();
	~DebugPhysics();

	bool Initialize();
	bool Render();

	bool AddShape(const char* shapeId, IDebugPhysicsSphere* shape);
	bool UpdateShape(const char* shapeId, IDebugPhysicsSphere* shape);

private:
	
	bool InitializeShape(DebugShapeType** shape, const char* shapeId, ID3D11Device* device);
	void CreateSphere(VertexType* vertices, unsigned long* indices, int lines, int vertexCount);
	
	bool UpdateShape(DebugShapeType* shape, ID3D11DeviceContext* context);
	void ReleaseShape(DebugShapeType** shape);
	bool RenderShape(ID3D11DeviceContext* deviceContext, DebugShapeType* sphere);

	void ReleaseAllShapes();

private:
	DebugShader* m_pDebugShader;

	DebugShapeType** m_pShapes;
	int m_shapeCount;
};