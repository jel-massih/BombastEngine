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
};

struct V2P
{
	float4 pos : SV_POSITION;
	float2 uv :  TEXCOORD0;
	float3 normal : NORMAL;
};

V2P VSMain(A2V input)
{
	V2P result;

	input.pos.w = 1.0f;

	result.pos = mul(input.pos, WorldMatrix);
	result.pos = mul(result.pos, ViewMatrix);
	result.pos = mul(result.pos, ProjectionMatrix);

	result.uv = input.uv;

	result.normal = mul(input.normal, (float3x3)WorldMatrix);
	result.normal = normalize(result.normal);

	return result;
}