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

#define BP_ASSERTf(expr, message, ...) \
	do { \
		if (!(expr)) \
		{ \
			char formattedMsg[1024]; \
			va_list args; \
			va_start(args, message); \
			vsprintf_s(formattedMsg, sizeof(formattedMsg), message, args); \
			assert(expr && formattedMsg); \
		} \
	} while (0) 