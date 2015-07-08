#pragma once

namespace bPhysics
{

#include "BpPreprocessor.h"

#ifdef BP_WINDOWS
	typedef signed __int64		i64;
	typedef signed int			i32;
	typedef signed short		i16;
	typedef signed char			i8;

	typedef unsigned __int64	u64;
	typedef unsigned int		u32;
	typedef unsigned short		u16;
	typedef unsigned char		u8;

	typedef float				f32;
	typedef double				f64;
#else
#	error Unknown platform!
#endif

}