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
		if (GetFullName(filename, fullPath, BOMBAST_MAX_PATH - 1))
		{

		}
	}
	return false;
}

bool Effect::GetFullName(const char* pShaderName, char* pShaderDestination, int length)
{
	if (pShaderName)
	{
		BE_ASSERTf(!strchr(pShaderName, '.'), "Do not include Extension in Effect Name: %s", pShaderName);
	}

	return false;
}