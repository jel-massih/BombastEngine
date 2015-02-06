Texture2D textureDiffuse : register(t0);
Texture2D normalTexture : register(t1);
SamplerState textureSampler : register(s0);

struct V2P
{
	float4 pos : SV_POSITION;
	float2 uv :  TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct P2F
{
	float4 color: SV_Target0;
	float4 normal: SV_Target1;
};

P2F FillGBufferPixelShader(V2P input)
{
	P2F result;
	float4 bumpMap;
	float3 bumpNormal;

	result.color = textureDiffuse.Sample(textureSampler, input.uv);

	bumpMap = normalTexture.Sample(textureSampler, input.uv);
	//Expand range from (0..1) to (-1..1)
	bumpMap = (bumpMap * 2.0f) - 1.0f;
	
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	bumpNormal = normalize(bumpNormal);
	result.normal = float4(bumpNormal, 1.0f);
	
	return result;
}