Texture2D Textures[2];
SamplerState Sampler : register(s0);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 LightmapPixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;
	float4 lightColor;
	float4 finalColor;

	color = Textures[0].Sample(Sampler, input.tex);
	lightColor = Textures[1].Sample(Sampler, input.tex);
	
	finalColor = color * lightColor;
	
	return finalColor;
}