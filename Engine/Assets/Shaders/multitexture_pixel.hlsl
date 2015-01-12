Texture2D shaderTextures[2];
SamplerState SampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor1;
	float4 textureColor2;
	float4 blendColor;

	textureColor1 = shaderTextures[0].Sample(SampleType, input.tex);
	textureColor2 = shaderTextures[1].Sample(SampleType, input.tex);

	blendColor = textureColor1 * textureColor2 * 2.0;

	blendColor = saturate(blendColor);

	return blendColor;
}
