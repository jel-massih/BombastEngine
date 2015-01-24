cbuffer MeshBuffer : register(b0)
{
	matrix LocalToProjected4x4;
	matrix LocalToWorld4x4;
	matrix WorldToView4x4;
};

struct VertexInputType
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct PixelInputType
{
	float4 pos : SV_POSITION;
	float2 uv :  TEXCOORD0;
	float3 worldNorm : TEXCOORD1;
	float viewDepth : TEXCOORD2;
};

PixelInputType VS(VertexInputType input)
{
	PixelInputType output;

	float4 wp = mul(input.pos, LocalToWorld4x4);
	output.pos = mul(input.pos, LocalToProjected4x4);

	output.uv = input.uv;
	
	output.worldNorm = mul(input.normal, (float3x3)LocalToWorld4x4);
	output.viewDepth = mul(wp, WorldToView4x4).z;

	return output;
}