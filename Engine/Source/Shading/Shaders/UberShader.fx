cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

#ifdef ENABLE_TEXTURE_INPUT
Texture2D shaderTexture;
SamplerState shaderTextureSampler;
#endif

#ifdef ENABLE_CAMERA_BUFFER
cbuffer CameraBuffer
{
	float3 eyePos;
	float padding;
}
#endif

struct VertexInputType
{
	float4 position : POSITION;

#ifdef VERT_INPUT_COLOR
	float4 color : COLOR;
#endif
	
#ifdef VERT_INPUT_TEX
	float2 tex : TEXCOORD0;
#endif

#ifdef VERT_INPUT_NORMAL
	float3 normal : NORMAL;
#endif
};

struct PixelInputType
{
	float4 position : SV_POSITION;

#ifdef VERT_INPUT_COLOR
	float4 color : COLOR;
#endif

#ifdef VERT_INPUT_TEX
	float2 tex : TEXCOORD0;
#endif

#ifdef VERT_INPUT_NORMAL
	float3 normal : NORMAL;
#endif

#ifdef PIXEL_INPUT_VIEWDIR
	float3 viewDirection : TEXCOORD1;
#endif
};

PixelInputType VSMain(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
#ifdef VERT_INPUT_COLOR
	output.color = input.color;
#endif

#ifdef VERT_INPUT_TEX
	output.tex = input.tex;
#endif

#ifdef VERT_INPUT_NORMAL
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);
#endif

#if defined(PIXEL_INPUT_VIEWDIR) && defined(ENABLE_CAMERA_BUFFER)
	float4 worldPosition;
	worldPosition = mul(input.position, worldMatrix);
	output.viewDirection = eyePos.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);
#endif

	return output;
}

#ifdef PIXEL_OUT_FLOAT4
float4 PSMain(PixelInputType input) : SV_TARGET
{
	float4 output = float4(0,0,0,0);

	#ifdef PIXEL_COLOR_DIRECT
	#ifndef VERT_INPUT_COLOR
	ERR_PIXEL_COLOR_DIRECT_requires_VERT_INPUT_COLOR_to_be_set
	#endif
	output = input.color;
	#endif

	#ifdef PIXEL_TEX_SAMPLE
	#if !defined(VERT_INPUT_TEX) || !defined(ENABLE_TEXTURE_INPUT)
	ERR_PIXEL_TEX_SAMPLE_requires_VERT_INPUT_TEX_and_ENABLE_TEXTURE_INPUT_to_be_set
	#endif
	output = shaderTexture.Sample(shaderTextureSampler, input.tex);
	#endif

	return output;
}
#elif defined(PIXEL_OUT_GBUFFER)
struct P2F
{
	float4 color: SV_Target0;
	float4 normal: SV_Target1;
};

P2F PSMain(PixelInputType input)
{
	P2F result;
	
	#ifdef PIXEL_TEX_SAMPLE
	#if !defined(VERT_INPUT_TEX) || !defined(ENABLE_TEXTURE_INPUT)
	ERR : PIXEL_TEX_SAMPLE requires VERT_INPUT_TEX and ENABLE_TEXTURE_INPUT to be set
	#endif
		result.color = shaderTexture.Sample(shaderTextureSampler, input.tex);
	#endif

	#ifndef VERT_INPUT_TEX
	ERR: PIXEL_OUT_GBUFFER Requires VERT_INPUT_TEX
	#endif
	result.normal = float4(input.normal, 1.0f);

	return result;
}
#endif