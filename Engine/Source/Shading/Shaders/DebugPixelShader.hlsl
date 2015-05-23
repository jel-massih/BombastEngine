cbuffer PixelBuffer
{
	float4 pixelColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
};

float4 DebugPixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;

	color.rgb = pixelColor.rgb;
	color.a = 1.0f;

	return color;
}
