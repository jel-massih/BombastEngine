#include "Effect.h"
#include <algorithm>

Effect* Effect::Create(const char* name)
{
	//@TODO: Check if effects full
	BE_LOG_GRAPHICS("Loading Effect: %s", name);

	Effect* found = LookupEffect(beStringHash(name));
	if (found) {
		return found;
	}

	found = BE_NEW Effect;
	if (found->Init(name)) {
		return found;
	}

	delete found;
	return NULL;
}

struct LookupByHash {
	bool operator()(Effect* const &a, const u32 &b) const {
		return a->GetHashCode() < b;
	}
};

Effect* Effect::LookupEffect(u32 hashCode)
{
	auto lower = std::lower_bound(sm_Effects.begin(), sm_Effects.end(), hashCode, LookupByHash());

	if (lower != sm_Effects.end()) {
		if ((*lower)->GetHashCode() == hashCode) 
		{
			return *lower;
		}
	}

	return NULL;
}

bool Effect::Init(const char* filename)
{
	Shutdown();

	//Load Effect
	if (filename)
	{
		char fullPath[BOMBAST_MAX_PATH];
		if (GetNameWithPath(filename, fullPath, BOMBAST_MAX_PATH - 1))
		{
			m_effectPath = fullPath;
			const char* srcName = beFilename(filename);
			BE_ASSERTf(strlen(srcName) < sizeof(m_effectName), "Effect name '%s' is too large for current code. (size %i)", srcName, sizeof(m_effectName));
			strncpy_s(m_effectName, srcName, strlen(srcName));
			m_nameHash = beStringHash(m_effectName);
			/*
			{
				Resource effectResource(fullPath);
				ResourceHandle* pVertexResHandle = g_pApp->m_pResourceCache->GetHandle(&effectResource);
			}*/
		}
	}
	return false;
}

bool Effect::GetNameWithPath(const char* pShaderName, char* pShaderDestination, int length)
{
	if (pShaderName)
	{
		BE_ASSERTf(!strchr(pShaderName, '.'), "Do not include Extension in Effect Name: %s", pShaderName);
		const char* extension = ".cso";
		memset(pShaderDestination, 0xFE, length);
		pShaderDestination[--length] = 0;
		sprintf_s(pShaderDestination, length, "%s.%s", pShaderName, extension);
		return true;
	}

	return false;
}

void Effect::Load(char* stream, int magic, const char* currentLoadingEffect)
{

}

void Effect::Shutdown()
{
	m_techniques.clear();
	m_vertexShaders.clear();
	m_pixelShaders.clear();

	//m_effectData
}

void Effect::StartEffectPass(int index, const EffectData& effectData) const
{

}

void Effect::EndEffectPass()
{

}

void Effect::BeginRender() const
{

}

void Effect::EndRender()
{

}

Program::Program()
{

}

Program::~Program()
{

}

bool Program::Load(const char* data, const int dataSize, const char* name)
{
	BE_LOG_GRAPHICS("Loading Program: [%s]", name);
	return true;
}

VertexProgram::VertexProgram()
{}

void VertexProgram::Load(const char* data, const int dataSize, const char* name)
{
	HRESULT hr;

	bool saveProgram = Program::Load(data, dataSize, name);

	hr = BEDEVICE.CreateVertexShader(data, dataSize, nullptr, &m_pProgram);
	
	if (FAILED(hr))
	{
		BE_ERROR("Failed to create Vertex Shader [%s]", name);
	}
}

VertexProgram::~VertexProgram()
{
	SAFE_RELEASE(m_pProgram);
}

PixelProgram::PixelProgram()
{}

void PixelProgram::Load(const char* data, const int dataSize, const char* name)
{
	HRESULT hr;

	bool saveProgram = Program::Load(data, dataSize, name);

	hr = BEDEVICE.CreatePixelShader(data, dataSize, nullptr, &m_pProgram);

	if (FAILED(hr))
	{
		BE_ERROR("Failed to create Pixel Shader [%s]", name);
	}
}

PixelProgram::~PixelProgram()
{
	SAFE_RELEASE(m_pProgram);
}