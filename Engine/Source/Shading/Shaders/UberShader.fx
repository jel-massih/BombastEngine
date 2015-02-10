cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

#if ENABLE_TEXTURE_INPUT
Texture2D shaderTexture;
SamplerState shaderTextureSampler;
#endif

#if ENABLE_CAMERA_BUFFER
cbuffer CameraBuffer
{
	float3 eyePos;
	float padding;
}
#endif

struct VertexInputType
{
	float4 position : POSITION;

#if VERT_INPUT_COLOR
	float4 color : COLOR;
#endif
	
#if VERT_INPUT_TEX
	float2 tex : TEXCOORD0;
#endif

#if VERT_INPUT_NORMAL
		float3 normal : NORMAL;
#endif
};

struct PixelInputType
{
#if VERT_INPUT_COLOR
	float4 color : COLOR;
#endif

#if VERT_INPUT_TEX
	float2 tex : TEXCOORD0;

#if VERT_INPUT_NORMAL
	float3 normal : NORMAL;
#endif

#if PIXEL_INPUT_VIEWDIR
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
	
#if VERT_INPUT_COLOR
	output.color = input.color;
#endif

#if VERT_INPUT_TEX
	output.tex = input.tex;
#endif

#if VERT_INPUT_NORMAL
	result.normal = mul(input.normal, (float3x3)WorldMatrix);
	result.normal = normalize(result.normal);
#endif

#if PIXEL_INPUT_VIEWDIR && CAMERA_BUFFER
	worldPosition = mul(input.position, worldMatrix);
	output.viewDirection = eyePos.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);
#endif

	return output;
}

#if PIXEL_OUT_FLOAT4
float4 PSMain(PixelInputType input) : SV_TARGET
{
	float4 output;

#if PIXEL_COLOR_DIRECT
#ifndef VERT_INPUT_COLOR
	ERR: PIXEL_COLOR_DIRECT requires VERT_INPUT_COLOR to be set
#endif
	output = input.color;
#endif

#if PIXEL_TEX_SAMPLE
#ifndef VERT_INPUT_TEX || ENABLE_TEXTURE_INPUT
	ERR: PIXEL_TEX_SAMPLE requires VERT_INPUT_TEX and ENABLE_TEXTURE_INPUT to be set
#endif
	output = shaderTexture.Sample(shaderTextureSampler, input.tex);
#endif

	return output;
}
#endif

#if PIXEL_OUT_GBUFFER
struct P2F
{
	float4 color: SV_Target0;
	float4 normal: SV_Target1;
};

P2F FillGBufferPixelShader(PixelInputType input)
{
	P2F result;
	
#if PIXEL_TEX_SAMPLE
#ifndef VERT_INPUT_TEX || ENABLE_TEXTURE_INPUT
ERR : PIXEL_TEX_SAMPLE requires VERT_INPUT_TEX and ENABLE_TEXTURE_INPUT to be set
#endif
	result.color = textureDiffuse.Sample(textureSampler, input.uv);
#endif

#ifndef VERT_INPUT_TEX
ERR: PIXEL_OUT_GBUFFER Requires VERT_INPUT_TEX
#endif
	  result.normal = float4(input.normal, 1.0f);

	return result;
}
#endif