#pragma once

#if __D3D11
typedef ID3D11VertexShader VertexShader;
typedef ID3D11PixelShader PixelShader;
#endif

class EffectData
{
public:
	EffectData();
	~EffectData();

	void Shutdown();
};

class Effect
{
	Effect();

	bool Init(const char* filename);

public:

	struct Technique
	{
		Technique();
		~Technique();
		void Load(char* stream);

		u32 nameHash;
		std::string name;
	};

	//Create Effect from Filename.
	static Effect* Create(const char* name);

	//Find effect, if available by hash code.
	static Effect* LookupEffect(u32 hashCode);

	//Get effect from static effect list by index
	static Effect* GetEffect(int index) { return sm_Effects[index]; }
	//Get count of loaded effects
	static int GetEffectCont() { return sm_Effects.size(); }

	//Get full name with path of the shader
	static bool GetNameWithPath(const char* pShaderName, char* pShaderDestination, int length);


public:
	~Effect();

	void Load(char* stream, int magic, const char* currentLoadingEffect);
	void Shutdown();

	u32 GetHashCode() const { return m_nameHash; }
	const char* GetEffectName() { return m_effectName; }
	const char* GetEffectPath() { return m_effectPath.c_str(); }

	void StartEffectPass(int index, const EffectData& effectData) const;
	void EndEffectPass();

	void BeginRender() const;
	void EndRender();

private:
	std::vector<Technique> m_techniques;
	std::vector<ID3D11VertexShader*> m_vertexShaders;
	std::vector<ID3D11PixelShader*> m_pixelShaders;

	char m_effectName[40];
	std::string m_effectPath;
	u32 m_nameHash;
	EffectData m_effectData;

	static std::vector<Effect*> sm_Effects;
};

//Program abstracts vertex/pixel shaders. Stores table to map effect params to shader params
class Program
{
public:
	Program();
	~Program();

	bool Load(const char* data, const int dataSize, const char* name);
};

class VertexProgram : Program
{
public:
	VertexProgram();
	~VertexProgram();
	void Load(const char* data, const int dataSize, const char* name);

	void Bind() const;

	VertexShader* GetProgram() const { return m_pProgram; }

private:
	VertexShader* m_pProgram;
};

class PixelProgram : Program
{
public:
	PixelProgram();
	~PixelProgram();
	void Load(const char* data, const int dataSize, const char* name);

	void Bind() const;

	PixelShader* GetProgram() const { return m_pProgram; }

private:
	PixelShader* m_pProgram;
};