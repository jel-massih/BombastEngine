Texture2D textureDiffuse : register(t0);
SamplerState textureSampler : register(s0);

struct V2P
{
	float4 pos : SV_POSITION;
	float2 uv :  TEXCOORD0;
	float3 normal : NORMAL;
};

struct P2F
{
	float4 color: SV_Target0;
	float4 normal: SV_Target1;
};

P2F FillGBufferPixelShader(V2P input)
{
	P2F result;

	result.color = textureDiffuse.Sample(textureSampler, input.uv);

	result.normal = float4(input.normal, 1.0f);
	
	return result;
}