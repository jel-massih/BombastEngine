#ifdef USE_ALBEDO_TEXTURE
Texture2D AlbedoTexture : register(t0);
sampler PointSampler : register(s0);
#endif

struct MaterialInfo
{
	float4 Emissive;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float SpecularPower;

	float3 Padding;
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
	Light light;
};

float4 CalcDiffuse(Light light, float3 lightDir, float3 normal)
{
	float intensity = saturate(dot(normal, lightDir));
	return light.Color * intensity;
};

float4 CalcSpecular(Light light, float3 viewDir, float3 lightDir, float3 normal)
{
	float4 result = float4(0,0,0,0);

	float intensity = saturate(dot(normal, lightDir));
	if (intensity > 0.0f) {
		float3 reflection = normalize(2 * intensity * normal - lightDir);
		float posDot = saturate(dot(reflection, viewDir));
		result = pow(posDot, Material.SpecularPower);
	}

	return result;
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
	LightingResult result;

	result = CalcDirectionalLight(light, viewDir, normal);

	result.Diffuse = saturate(result.Diffuse);
	result.Specular = saturate(result.Specular);

	return result;
};

struct V2P
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
	float3 viewDirection : TEXCOORD1;
};

float4 PSMain(V2P input) : SV_TARGET
{
	LightingResult lit = CalcLighting(input.viewDirection, normalize(input.Normal));

	float4 emissive = Material.Emissive;
	float4 ambient = Material.Ambient * GlobalAmbient;
	float4 diffuse = Material.Diffuse * lit.Diffuse;
	float4 specular = Material.Specular * lit.Specular;

	float4 texColor = { 1, 1, 1, 1 };

#ifdef USE_ALBEDO_TEXTURE
	texColor = AlbedoTexture.Sample(PointSampler, input.TexCoord);
#endif

	float4 finalColor = (emissive + ambient + diffuse) * texColor;

	finalColor = saturate(finalColor + specular);

	return finalColor;
}

struct P2F
{
	float4 color: SV_Target0;
	float4 normal: SV_Target1;
};

struct V2PDeferred
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

P2F PSMainDeferred(V2PDeferred input) : SV_TARGET
{
	P2F result;

	float4 emissive = Material.Emissive;
	float4 ambient = Material.Ambient * GlobalAmbient;
	float4 diffuse = Material.Diffuse;

	float4 texColor = { 1, 1, 1, 1 };

#ifdef USE_ALBEDO_TEXTURE
	texColor = AlbedoTexture.Sample(PointSampler, input.TexCoord);
#endif

	result.color = (emissive + ambient + diffuse) * texColor;
	result.color = saturate(result.color);

	result.normal = float4(input.Normal, 1.0f);
	
	return result;
}