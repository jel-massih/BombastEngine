#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "../BombastEngineStd.h"
#include "../Graphics2D/TextureClass.h"

class ModelClass
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

public:
	ModelClass();
	ModelClass(const ModelClass& other);
	~ModelClass();

	bool Initialize(ID3D11Device* device, std::string textureFilepath);
	void Shutdown();
	void Render(ID3D11DeviceContext* context);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* context);

	bool LoadTexture(ID3D11Device* device, std::string textureFilepath);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_pVertexBuffer, *m_pIndexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_pTexture;
};

#endif