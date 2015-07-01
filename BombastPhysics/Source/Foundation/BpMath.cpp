#include "BpMath.h"
#include <limits>

using namespace bPhysics;

 const BpVec3 BpVec3::g_InvalidBpVec3 = BpVec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
 const BpVec2 BpVec2::g_InvalidBpVec2 = BpVec2(-FLT_MAX, -FLT_MAX);
 
 const BpMat4x4 BpMat4x4::g_InvalidBpMat4x4 = BpMat4x4(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX,
													   -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX,
													   -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX,
													   -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
 const BpMat4x4 BpMat4x4::g_Identity = BpMat4x4(1, 0, 0, 0, 0, 1, 0, 0 ,0, 0, 1, 0, 0, 0, 0, 1);