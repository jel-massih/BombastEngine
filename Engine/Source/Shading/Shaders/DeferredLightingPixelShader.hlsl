//Take in 2 Render Targets Textures with Color and Normal Information
Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);

//Sampler for the exact per-pixel data values from render textures
SamplerState PointSampler : register(s0);

cbuffer LightBuffer : register(b0)
{
	float3 lightDirection;
	float padding;
};

struct V2P
{
	float4 pos: SV_POSITION;
	float2 uv: TEXCOORD0;
};

float4 DeferredLightingPS(V2P input) : SV_TARGET
{
	float4 colors;
	float4 normals;
	float3 lightDir;
	float lightIntensity;
	float4 outColor;

	//Sample Color from diffuse Render Texture
	colors = colorTexture.Sample(PointSampler, input.uv);
	//Sample normal from normals Render Texture
	normals = normalTexture.Sample(PointSampler, input.uv);

	lightDir = -lightDirection;

	lightIntensity = saturate(dot(normals.xyz, lightDir));

	outColor = saturate(colors * lightIntensity);

	return outColor;
}