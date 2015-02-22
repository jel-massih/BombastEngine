#pragma once

class Effect
{
	Effect();

	bool Init(const char* filename);
	void Shutdown();

public:
	static const int MaxEffects = 512;

	struct Technique
	{
		Technique();
		~Technique();
		void Load(char* stream);

		unsigned int nameHash;
		std::string name;
	};

	void Load(char* stream, int magic, const char* currentLoadingEffect);

	//Create Effect from Filename.
	static Effect* Create(const char* name);

	//Create Effect from Filename. First loaded 
	static Effect* CreateWithFallback(const char* name);

	//Find effect, if available by hash code.
	static Effect* LookupEffect(u32 hashCode);

	static bool GetFullName(const char* pShaderName, char* pShaderDestination, int length);

	~Effect();

	u32 GetHashCode() const { return m_nameHash; }

private:
	char m_EffectName[40];
	std::string m_effectPath;
	u32 m_nameHash;


	static std::vector<Effect*> sm_Effects;
};