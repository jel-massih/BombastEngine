cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
	float3 eyePos;
	float padding;
}

struct VertexInputType
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
	float3 viewDirection : TEXCOORD1;
};

PixelInputType LitTexturedVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;

	output.tex = input.tex;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.normal = mul(input.normal, worldMatrix);
	output.normal = normalize(output.normal);

	worldPosition = mul(eyePos, worldMatrix);
	output.viewDirection = eyePos.xyz - worldMatrix.xyz;
	output.viewDirection = normalize(output.viewDirection);

	return output;
}