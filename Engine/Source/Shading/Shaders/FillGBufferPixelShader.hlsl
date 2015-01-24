Texture2D textureDiffuse : register(t0);
SamplerState textureSampler : register(s0);

cbuffer SubMeshBuffer : register(b0)
{
	float3 DiffuseColor;
	int bTextured;
};

struct PixelInputType
{
	float4 pos : SV_POSITION;
	float2 uv :  TEXCOORD0;
	float3 worldNorm : TEXCOORD1;
	float viewDepth : TEXCOORD2;
};

struct FragmentInputType
{
	float4 fragment1 : SV_Target0;
	float4 fragment2 : SV_Target1;
	uint fragment3 : SV_Target2;
};

FragmentInputType PS(PixelInputType input, uint coverage : SV_Coverage) : SV_TARGET
{
	FragmentInputType output;

	float3 worldNormal = normalize(input.worldNorm);
	float viewDepth = input.viewDepth / 1000.0f;

	float3 diffuse = bTextured > 0 ? textureDiffuse.Sample(textureSampler, float2(1, 1) - input.uv).rgb : DiffuseColor;
	float edge = coverage != 0xf;

	output.fragment1 = float4(worldNormal, viewDepth);
	output.fragment2 = float4(diffuse, edge);
	output.fragment3 = coverage;

	return output;
}