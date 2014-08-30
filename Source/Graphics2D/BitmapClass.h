#ifndef BITMAP_CLASS_H
#define BITMAP_CLASS_H

#include "../BombastEngineStd.h"
#include "TextureClass.h"

class BitmapClass {
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
public:
	BitmapClass();
	BitmapClass(const BitmapClass& other); //Copy Constructor
	~BitmapClass();

	bool Initialize(ID3D11Device* device, std::string textureFilename, int bitmapWidth, int bipmapHeight);
	void Shutdown();
	
	bool Render(ID3D11DeviceContext* deviceContext);
	
	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

	void SetPosition(int x, int y);
	void Translate(int dx, int dy);

	int GetBitmapWidth();
	int GetBitmapHeight();

private:
	bool LoadTexture(ID3D11Device* device, std::string filename);
	void ReleaseTexture();

	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	int m_bitmapWidth;
	int m_bitmapHeight;

	int m_previousPosX;
	int m_previousPosY;

	int m_posX;
	int m_posY;

	int m_vertexCount; //Number of Vertices in Vertex Array
	int m_indexCount; //Number of Indices in Index array

	ID3D11Buffer *m_pVertexBuffer, *m_pIndexBuffer;
	
	TextureClass *m_pTexture;
};

#endif