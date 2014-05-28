#ifndef BITMAP_CLASS_H
#define BITMAP_CLASS_H

#include "../BombastEngineStd.h"
#include "TextureClass.h"

class BitmapClass {
public:
	BitmapClass();
	BitmapClass(const BitmapClass& other); //Copy Constructor
	~BitmapClass();

	bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bipmapHeight);
	void Shutdown();
	
	bool Render(ID3D11DeviceContext* deviceContext, int posX, int posY);
	
	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTexture(ID3D11Device* device, WCHAR* filename);
	void ReleaseTexture();

	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	int m_screenWidth;
	int m_screenHeight;
	int m_bitmapWidth;
	int m_bitmapHeight;

	int m_previousPosX;
	int m_previousPosY;

	int m_vertexCount; //Number of Vertices in Vertex Array
	int m_indexCount; //Number of Indices in Index array

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	
	TextureClass* m_Texture;

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
};

#endif