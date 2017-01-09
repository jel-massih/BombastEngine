#pragma once

class DebugShader;
class DebugPhysicsLine;
class DebugPhysicsTriangle;
class DebugPhysicsSphere;
class DebugPhysicsBox;
class DebugPhysicsCapsule;
class DebugPhysicsLineList;
class DebugPhysicsTriangleList;

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
		D3D11_PRIMITIVE_TOPOLOGY topology;
	};

public:
	struct VertexType
	{
		XMFLOAT3 position;

		VertexType() {}

		VertexType(XMFLOAT3 pos)
		{
			position = pos;
		}
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
	bool CreateCapsule(VertexType** vertices, unsigned long** indices, DebugShapeType* shape, DebugPhysicsCapsule* capsule);
	bool CreateLine(VertexType** vertices, unsigned long** indices, DebugShapeType* shape, DebugPhysicsLine* line);
	bool CreateTriangle(VertexType** vertices, unsigned long** indices, DebugShapeType* shape, DebugPhysicsTriangle* triangle);
	bool CreateLineList(VertexType** vertices, unsigned long** indices, DebugShapeType* shape, DebugPhysicsLineList* lineList);
	bool CreateTriangleList(VertexType** vertices, unsigned long** indices, DebugShapeType* shape, DebugPhysicsTriangleList* triangleList);

	bool UpdateShape(DebugShapeType* shape, ID3D11DeviceContext* context);
	void ReleaseShape(DebugShapeType** shape);
	bool RenderShape(ID3D11DeviceContext* deviceContext, DebugShapeType* sphere);

	void ReleaseAllShapes();

private:
	DebugShader* m_pDebugShader;

	DebugShapeType** m_pShapes;
	int m_shapeCount;
};

class PhysXPhysicsColor
{
public:
	int r, g, b, a;

	PhysXPhysicsColor(int rgba) : a(255), r(rgba & 0xff), g((rgba >> 8) & 0xff), b((rgba >> 16) & 0xff) {}
};

class DebugPhysicsLine : public IDebugPhysicsShape
{
public:
	DebugPhysicsLine(Mat4x4 transform, Vec3 pos0, Vec3 pos1, Vec3 color) : m_transform(transform), m_pos0(pos0), m_pos1(pos1), m_color(color) {}

	virtual Vec3 VGetColor() override { return m_color; }
	virtual Mat4x4 VGetTransform() override { return m_transform; }
	virtual DebugPhysicsShapeType VGetShapeType() override { return DebugPhysicsShapeType::LINE; }

public:
	Vec3 m_pos0;
	Vec3 m_pos1;

private:
	Mat4x4 m_transform;
	Vec3 m_color;
};

class DebugPhysicsTriangle : public IDebugPhysicsShape
{
public:
	DebugPhysicsTriangle(Mat4x4 transform, Vec3 pos0, Vec3 pos1, Vec3 pos2, Vec3 color) : m_transform(transform), m_pos0(pos0), m_pos1(pos1), m_pos2(pos2), m_color(color) {}

	virtual Vec3 VGetColor() override { return m_color; }
	virtual Mat4x4 VGetTransform() override { return m_transform; }
	virtual DebugPhysicsShapeType VGetShapeType() override { return DebugPhysicsShapeType::TRIANGLE; }

public:
	Vec3 m_pos0;
	Vec3 m_pos1;
	Vec3 m_pos2;

private:
	Mat4x4 m_transform;
	Vec3 m_color;
};

class DebugPhysicsSphere : public IDebugPhysicsShape
{
public:
	DebugPhysicsSphere(Mat4x4 transform, Vec3 color, float radius) : m_transform(transform), m_color(color), m_radius(radius) {}

	virtual Vec3 VGetColor() { return m_color; }
	virtual DebugPhysicsShapeType VGetShapeType() { return DebugPhysicsShapeType::SPHERE; }
	float VGetRadius() { return m_radius; }

	virtual Mat4x4 VGetTransform() override { return m_transform; }

private:
	Vec3 m_color;
	float m_radius;
	Mat4x4 m_transform;
};

class DebugPhysicsBox : public IDebugPhysicsShape
{
public:
	DebugPhysicsBox(Mat4x4 transform, Vec3 color, Vec3 extent) : m_transform(transform), m_color(color), m_extent(extent) {}

	virtual Vec3 VGetColor() { return m_color; }
	virtual DebugPhysicsShapeType VGetShapeType() { return DebugPhysicsShapeType::BOX; }
	Vec3 VGetExtent() { return m_extent; }

	virtual Mat4x4 VGetTransform() override { return m_transform; }

private:
	Vec3 m_color, m_extent;
	Mat4x4 m_transform;
};

class DebugPhysicsCapsule : public IDebugPhysicsShape
{
public:
	DebugPhysicsCapsule(Mat4x4 transform, Vec3 color, float radius, float halfHeight) : m_transform(transform), m_color(color), m_radius(radius), m_halfHeight(halfHeight) {}

	virtual Vec3 VGetColor() { return m_color; }
	virtual DebugPhysicsShapeType VGetShapeType() { return DebugPhysicsShapeType::CAPSULE; }
	float VGetRadius() { return m_radius; }
	float VGetHalfHeight() { return m_halfHeight; }

	virtual Mat4x4 VGetTransform() override { return m_transform; }

private:
	Vec3 m_color;
	float m_radius, m_halfHeight;
	Mat4x4 m_transform;
};

class DebugPhysicsLineList : public IDebugPhysicsShape
{
public:
	DebugPhysicsLineList(Vec3 color) : m_color(color), m_vertices(nullptr) {}
	virtual ~DebugPhysicsLineList() { SAFE_DELETE_ARRAY(m_vertices); }

	virtual Vec3 VGetColor() { return m_color; }
	virtual DebugPhysicsShapeType VGetShapeType() { return DebugPhysicsShapeType::LINE_LIST; }

	virtual Mat4x4 VGetTransform() override { return Mat4x4::g_Identity; }

	DebugPhysics::VertexType* GetVertices() { return m_vertices; }
	void SetVertices(DebugPhysics::VertexType* vertices) { m_vertices = vertices; }

	int GetVertexCount() { return m_vertexCount; }
	void SetVertexCount(int vertexCount) { m_vertexCount = vertexCount; }

private:
	Vec3 m_color;

	DebugPhysics::VertexType* m_vertices;
	int m_vertexCount;
};

class DebugPhysicsTriangleList : public IDebugPhysicsShape
{
public:
	DebugPhysicsTriangleList(Vec3 color) : m_color(color), m_vertices(nullptr) {}
	virtual ~DebugPhysicsTriangleList() { SAFE_DELETE_ARRAY(m_vertices); }

	virtual Vec3 VGetColor() { return m_color; }
	virtual DebugPhysicsShapeType VGetShapeType() { return DebugPhysicsShapeType::TRIANGLE_LIST; }

	virtual Mat4x4 VGetTransform() override { return Mat4x4::g_Identity; }

	DebugPhysics::VertexType* GetVertices() { return m_vertices; }
	void SetVertices(DebugPhysics::VertexType* vertices) { m_vertices = vertices; }
	
	int GetVertexCount() { return m_vertexCount; }
	void SetVertexCount(int vertexCount) { m_vertexCount = vertexCount; }

private:
	Vec3 m_color;

	DebugPhysics::VertexType* m_vertices;
	int m_vertexCount;
};