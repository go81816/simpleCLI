#pragma warning (disable : 4786) 
#include <dlfcn.h>
#include <string>

#define DYNAMIC_MODULE_EXT ".so"
bool loadModule( const char* szModuleName ) ;
bool unloadModule( const char* szModuleName ) ;
void findSymbol( const char* szModuleName, const char* szProcName, EXTENSIBLEAPPPROC& p, bool bLoad=true ) ;
 
