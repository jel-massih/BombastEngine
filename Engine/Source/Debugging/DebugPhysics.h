#pragma once

class DebugShader;
class BombastDebugPhysicsCapsule;

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

	bool AddShape(const char* shapeId, IDebugPhysicsShape* shape);
	bool UpdateShape(const char* shapeId, IDebugPhysicsShape* shape);

private:
	
	bool InitializeShape(DebugShapeType** shape, const char* shapeId, IDebugPhysicsShape* sourceShape, ID3D11Device* device);
	bool CreateSphere(VertexType** vertices, unsigned long** indices, DebugShapeType* shape);
	bool CreateBox(VertexType** vertices, unsigned long** indices, DebugShapeType* shape);
	bool CreateCapsule(VertexType** vertices, unsigned long** indices, DebugShapeType* shape, BombastDebugPhysicsCapsule* capsule);

	bool UpdateShape(DebugShapeType* shape, ID3D11DeviceContext* context);
	void ReleaseShape(DebugShapeType** shape);
	bool RenderShape(ID3D11DeviceContext* deviceContext, DebugShapeType* sphere);

	void ReleaseAllShapes();

private:
	DebugShader* m_pDebugShader;

	DebugShapeType** m_pShapes;
	int m_shapeCount;
};