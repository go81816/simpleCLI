// understood function prototype  - this type is used by platform specific code must be defined  prior to include
typedef int (*EXTENSIBLEAPPPROC)(const char* pCfgData, const char* pParamData); 

#if defined(WIN32)
#include "win32.h"
#elif defined(MACOS_X_UNIX)
//
#elif defined(LINUX)
#include "linux.h"
#else
//
#endif
