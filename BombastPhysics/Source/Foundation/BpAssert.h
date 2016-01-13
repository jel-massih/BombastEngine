#pragma once

#include <cassert>
#include <cstdarg>
#include <stdio.h>

//Add actual implementation with an assertion handler
#define BP_ASSERT(expr) \
	do { \
		if (!(expr)) \
						{ \
			assert(expr); \
						} \
			} while (0) \

#define BP_ASSERTf(expr, message) \
	do { \
		if (!(expr)) \
		{ \
			assert(expr && message); \
		} \
	} while (0) 