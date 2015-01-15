#define MAX_LIGHTS 4

Texture2D Texture : register(t0);
sampler Sampler : register(s0);

struct MaterialInfo
{
	float4 Emissive;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float SpecularPower;

	bool UseTexture;
	float2 Padding;
};

cbuffer MaterialProperties : register(b0)
{
	MaterialInfo Material;
};

struct Light
{
	float4 Color;
	float3 Direction;
	bool Enabled;
};

cbuffer LightProperties : register(b1)
{
	float4 EyePosition;
	float4 GlobalAmbient;
	Light Lights[MAX_LIGHTS];
};

float CalcDiffuse(Light light, float3 lightDir, float3 normal)
{
	float intensity = saturate(dot(normal, lightDir));
	return light.Color * intensity;
};

float CalcSpecular(Light light, float3 eyePos, float3 lightDir, float3 normal)
{
	float3 R = normalize(reflect(-lightDir, normal));
	float posDot = saturate(dot(R, eyePos));
	return light.Color * pow(posDot, Material.SpecularPower);
};

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

LightingResult CalcDirectionalLight(Light light, float3 eyePos, float4 pixelPos, float3 normal)
{
	LightingResult result;

	float3 lightDir = -light.Direction.xyz;

	result.Diffuse = CalcDiffuse(light, lightDir, normal); 
	result.Specular = CalcSpecular(light, eyePos, lightDir, normal);

	return result;
};

LightingResult CalcLighting(float4 pixelPos, float3 normal)
{
	float3 eyePos = normalize(EyePosition - pixelPos).xyz;
	LightingResult totalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
	
	[unroll]
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

		if (!Lights[i].Enabled) continue;

		result = CalcDirectionalLight(Lights[i], eyePos, pixelPos, normal);
		totalResult.Diffuse += result.Diffuse;
		totalResult.Specular += result.Specular;
	}

	totalResult.Diffuse = saturate(totalResult.Diffuse);
	totalResult.Specular = saturate(totalResult.Specular);

	return totalResult;
};

struct PixelShaderInput
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
	float3 viewDirection : TEXCOORD1;
};

float4 LitTexturedPixelShader(PixelShaderInput input) : SV_TARGET
{
	LightingResult lit = CalcLighting(input.Position, normalize(input.Normal));
	
	float4 emissive = Material.Emissive;
	float4 ambient = Material.Ambient * GlobalAmbient;
	float4 diffuse = Material.Diffuse * lit.Diffuse;
	float4 specular = Material.Specular * lit.Specular;
	
	float4 texColor = {1,1,1,1};

	if (Material.UseTexture)
	{
		texColor = Texture.Sample(Sampler, input.TexCoord);
	}

	float4 finalColor = (emissive + ambient + diffuse + specular) * texColor;
	
	return finalColor;
}