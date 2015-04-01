#pragma once

#include <cassert>

//Add actual implementation with an assertion handler
#define BP_ASSERT(expr) assert(expr)

#define BP_ASSERT(expr, msg) assert(expr)