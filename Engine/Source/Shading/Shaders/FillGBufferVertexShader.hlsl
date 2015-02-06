cbuffer MatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct A2V
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct V2P
{
	float4 pos : SV_POSITION;
	float2 uv :  TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

V2P VS(A2V input)
{
	V2P result;

	input.pos.w = 1.0f;

	result.pos = mul(input.pos, WorldMatrix);
	result.pos = mul(result.pos, ViewMatrix);
	result.pos = mul(result.pos, ProjectionMatrix);

	result.uv = input.uv;
	
	result.normal = mul(input.normal, (float3x3)WorldMatrix);
	result.normal = normalize(result.normal);

	result.tangent = mul(input.tangent, (float3x3)WorldMatrix);
	result.tangent = normalize(result.tangent);

	result.binormal = mul(input.binormal, (float3x3)WorldMatrix);
	result.binormal = normalize(result.binormal);

	return result;
}