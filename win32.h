#pragma warning (disable : 4786) 
#include <windows.h>
#include <string>

int getRegOption( LPCTSTR szApp, DWORD idx, std::string& sKey, std::string& sVal ) ;
int getRegOption( LPCTSTR szApp, LPCTSTR szValName, std::string& sVal ) ;

#define DYNAMIC_MODULE_EXT ".dll"
bool loadModule( LPCTSTR szModuleName ) ;
bool unloadModule( LPCTSTR szModuleName ) ;
void findSymbol( LPCTSTR szModuleName, LPCSTR szProcName, EXTENSIBLEAPPPROC& p, bool bLoad=true ) ;
 