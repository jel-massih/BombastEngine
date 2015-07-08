#pragma once

#include <crtdbg.h>

#if defined(_DEBUG)
#define BP_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#define BE_NEW new
#endif

#if !defined(BP_SAFE_DELETE)
#define BP_SAFE_DELETE(x) if(x) delete x; x=NULL;
#endif

#if !defined(BP_SAFE_DELETE_ARRAY)
#define BP_SAFE_DELETE_ARRAY(x) if(x) delete [] x; x=NULL;
#endif