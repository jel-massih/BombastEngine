#pragma once

namespace beShading {

	//===========Buffers================
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	static_assert((sizeof(MatrixBufferType) % 16) == 0, "MatrixBufferType CB size not padded correctly");

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	static_assert((sizeof(CameraBufferType) % 16) == 0, "CameraBufferType CB size not padded correctly");

	struct MaterialBufferType
	{
		XMFLOAT4 emissive;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		float specularPower;
		XMFLOAT3 padding;
	};
	
	static_assert((sizeof(MaterialBufferType) % 16) == 0, "MaterialBufferType CB size not padded correctly");

	struct LightProperties
	{
		XMFLOAT4 lightColor;
		XMFLOAT3 lightDirection;
		bool enabled;
	};

	static_assert((sizeof(LightProperties) % 16) == 0, "LightProperties CB size not padded correctly");

	struct LightBufferType
	{
		XMFLOAT4 eyePosition;
		XMFLOAT4 globalAmbient;
		LightProperties light;
	};

	static_assert((sizeof(LightBufferType) % 16) == 0, "LightBufferType CB size not padded correctly");
};
