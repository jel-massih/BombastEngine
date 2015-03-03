cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct A2V
{
	float4 position : POSITION;
	float2 uv : TEXCOORD0;
};

struct V2P
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

V2P DeferredLightingVS(A2V input)
{
	V2P result;

	input.position.w = 1.0f;

	result.position = mul(input.position, worldMatrix);
	result.position = mul(result.position, viewMatrix);
	result.position = mul(result.position, projectionMatrix);

	result.uv = input.uv;

	return result;
}