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

struct Light
{
	float4 Color;
	float3 Direction;
	bool Enabled;
};

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

float4 CalcDiffuse(Light light, float3 lightDir, float3 normal)
{
	float intensity = saturate(dot(normal, lightDir));
	return light.Color * intensity;
};

float4 CalcSpecular(Light light, float3 viewDir, float3 lightDir, float3 normal, float specularPower)
{
	float intensity = saturate(dot(normal, lightDir));
	if (intensity > 0.0f) {
		float3 reflection = normalize(2 * intensity * normal - lightDir);
		float posDot = saturate(dot(reflection, viewDir));
		return pow(posDot, specularPower);
	}

	return float4(0, 0, 0, 0);
};

LightingResult CalcDirectionalLight(Light light, float3 viewDir, float3 normal, float specPower)
{
	LightingResult result;

	float3 lightDir = -light.Direction.xyz;

	result.Diffuse = CalcDiffuse(light, lightDir, normal); 
	result.Specular = CalcSpecular(light, viewDir, lightDir, normal, specPower);

	return result;
};

LightingResult CalcLighting(Light light, float3 viewDir, float3 normal, float specPower)
{
	LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

	result = CalcDirectionalLight(light, viewDir, normal, specPower);

	result.Diffuse = saturate(result.Diffuse);
	result.Specular = saturate(result.Specular);

	return result;
};

float4 CalcFinalMaterialResult(float4 input, LightingResult lighting, MaterialInfo mat, float4 globalAmbient)
{
	float4 result;

	float4 emissive = mat.Emissive;
	float4 ambient = mat.Ambient * globalAmbient;
	float4 diffuse = mat.Diffuse * lighting.Diffuse;
	float4 specular = mat.Specular * lighting.Specular;

	result = (emissive + ambient + diffuse) * input;

	result = saturate(result + specular);

	return result;
};