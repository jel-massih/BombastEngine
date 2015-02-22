#ifdef USE_ALBEDO_TEXTURE
	#define ALBEDO_TEXTURE 1
#else
	#define ALBEDO_TEXTURE 0
#endif

#ifdef USE_NORMAL_MAP
	#define NORMAL_MAP 1
#else
	#define NORMAL_MAP 0
#endif

#ifdef USE_CAMERA_BUFFER
	#define CAMERA_BUFFER 1
#else
	#define CAMERA_BUFFER 0
#endif

#if ALBEDO_TEXTURE || NORMAL_MAP
	#define USE_POINT_SAMPLER
#endif

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);

SamplerState textureSampler : register(s0);

cbuffer CameraBuffer : register(b1)
{
	float3 eyePos;
	float padding;
};

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

#ifdef PIXEL_INPUT_VIEWDIR
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

	#ifdef VERT_INPUT_COLOR
		output = input.color;
	#endif

	#ifdef ALBEDO_TEXTURE
		#ifdef VERT_INPUT_TEX
			output = albedoTexture.Sample(textureSampler, input.tex);
		#else
			ERR_PIXEL_TEX_SAMPLE_requires_VERT_INPUT_TEX_and_ENABLE_TEXTURE_INPUT_to_be_set
		#endif
	#endif

	#ifdef NORMAL_MAP
		#ifndef VERT_INPUT_TEX
			ERR_PIXEL_TEX_SAMPLE_requires_VERT_INPUT_TEX_and_ENABLE_TEXTURE_INPUT_to_be_set
		#endif
		float4 normals = normalTexture.Sample(textureSampler, input.tex);
		float4 lightIntensity = saturate(dot(normals.xyz, -eyePos));

		output = saturate(output * lightIntensity);
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

	#ifdef ALBEDO_TEXTURE
		#ifdef VERT_INPUT_TEX
			result.color = albedoTexture.Sample(textureSampler, input.tex);
		#else
			ERR: PIXEL_TEX_SAMPLE requires VERT_INPUT_TEX and ENABLE_TEXTURE_INPUT to be set
		#endif
	#endif

	result.normal = float4(input.normal, 1.0f);

	return result;
}
#endif