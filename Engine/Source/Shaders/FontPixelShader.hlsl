Texture2D shaderTexture;
SamplerState SampleType;

cbuffer PixelBuffer
{
	float4 pixelColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 FontPixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;

	color = shaderTexture.Sample(SampleType, input.tex);

	if (color.r == 0.0f)
	{
		color.a = 0.0f;
	}
	else
	{
		color.rgb = pixelColor.rgb;
		color.a = 1.0f;
	}

	return color;
}
