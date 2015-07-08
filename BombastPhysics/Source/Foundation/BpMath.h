#pragma once
#include <cmath>
#include "BpSimpleTypes.h"

namespace bPhysics
{
	//Enum for identity constructor flag for quats/transforms/matrices
	enum BpIDENTITY { BpIdentity };

	enum BpINVALID { BpInvalid };

	const f32  BP_PI = 3.14159265358979f;

	//====== Unit Conversions =========
	inline f32 BPConvertToRadians(f32 fDegrees) { return fDegrees * (BP_PI / 180.0f); }
	inline f32 BPConvertToDegrees(f32 fRadians) { return fRadians * (180.0f / BP_PI); }

	//====== Misc Helpers ===========
	inline bool BpIsFinite(f32 f) { return isfinite(f); }

	inline f32 BpPow(f32 x, f32 y) { return ::powf(x, y); }
}