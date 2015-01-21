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

float4 CalcDiffuse(Light light, float3 lightDir, float3 normal)
{
	float intensity = saturate(dot(normal, lightDir));
	return light.Color * intensity;
};

float4 CalcSpecular(Light light, float3 viewDir, float3 lightDir, float3 normal)
{
	float intensity = saturate(dot(normal, lightDir));
	if (intensity > 0.0f) {
		float3 reflection = normalize(2 * intensity * normal - lightDir);
		float posDot = saturate(dot(reflection, viewDir));
		return pow(posDot, Material.SpecularPower);
	}

	return float4(0, 0, 0, 0);
};

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

LightingResult CalcDirectionalLight(Light light, float3 viewDir, float3 normal)
{
	LightingResult result;

	float3 lightDir = -light.Direction.xyz;

	result.Diffuse = CalcDiffuse(light, lightDir, normal); 
	result.Specular = CalcSpecular(light, viewDir, lightDir, normal);

	return result;
};

LightingResult CalcLighting(float3 viewDir, float3 normal)
{
	LightingResult totalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

	[unroll]
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

		if (!Lights[i].Enabled) continue;

		result = CalcDirectionalLight(Lights[i], viewDir, normal);
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
	LightingResult lit = CalcLighting(input.viewDirection, normalize(input.Normal));
	
	float4 emissive = Material.Emissive;
	float4 ambient = Material.Ambient * GlobalAmbient;
	float4 diffuse = Material.Diffuse * lit.Diffuse;
	float4 specular = Material.Specular * lit.Specular;
	
	float4 texColor = {1,1,1,1};

	if (Material.UseTexture)
	{
		texColor = Texture.Sample(Sampler, input.TexCoord);
	}

	float4 finalColor = (emissive + ambient + diffuse) * texColor;

	finalColor = saturate(finalColor + specular);

	return finalColor;
}