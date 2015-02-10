#pragma once

class Effect
{
	Effect();

	bool Init(const char* filename);

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

	~Effect();
};