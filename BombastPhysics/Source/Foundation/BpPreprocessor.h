#pragma once

#ifdef _MSC_VER
#	define BP_VC
#else
#	error "Unknown Compiler"
#endif

#ifdef BP_VC
#	ifdef _M_IX86
#		define BP_X86
#		define BP_WINDOWS
#	else
#		error "Unkown Platform"
#	endif
#endif