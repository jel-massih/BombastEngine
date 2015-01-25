cbuffer LightPassBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct A2V
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct V2P
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

V2P DeferredLightingVS(A2V input)
{
	V2P result;

	input.pos.w = 1.0f;

	result.pos = mul(input.pos, worldMatrix);
	result.pos = mul(result.pos, viewMatrix);
	result.pos = mul(result.pos, projectionMatrix);

	result.uv = input.uv;

	return result;
}