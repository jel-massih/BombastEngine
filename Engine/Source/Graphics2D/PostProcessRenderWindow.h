#pragma once

class PostProcessRenderWindow
{
private:
	struct VertexType
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

public:
	PostProcessRenderWindow() : m_pVertexBuffer(nullptr), m_pIndexBuffer(nullptr), m_vertexCount(0), m_indexCount(0) {}
	~PostProcessRenderWindow();

	bool Initialize(ID3D11Device* device, int width, int height);
	void Render(ID3D11DeviceContext* context);

	int GetIndexCount() const { return m_indexCount; }

private:
	bool InitializeBuffers(ID3D11Device* device, int width, int height);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* context);

private:
	ID3D11Buffer* m_pVertexBuffer, *m_pIndexBuffer;
	int m_vertexCount, m_indexCount;
};