#if defined(LINUX)

#include "simpleCLI_platformspec.h"

#include <string>
#include <map>

// ************************************************************************************************
// **********************************Dynamic Loading Functions*************************************
// ************************************************************************************************
static std::map<std::string,void*> g_LoadedModuleMap;
bool loadModule( const char* szModuleName ) {

	int iFlags = RTLD_LAZY;  //RTLD_GLOBAL
	void* hMod = dlopen( szModuleName, iFlags );

	bool bRet = false;
	if( hMod!=NULL ) {
		std::string sModName = szModuleName;
		std::pair<std::map<std::string,void*>::iterator,bool > pr;
		pr = g_LoadedModuleMap.insert( std::pair<std::string,void*>(sModName,hMod) );
		bRet = pr.second;
	}

	return bRet;
}

bool unloadModule( const char* szModuleName ) {

	std::string stemp = szModuleName;
	std::map<std::string,void*>::iterator it = g_LoadedModuleMap.find( stemp );

	int ret = 0;   //if a module is not found, should this return true or false?
	if( it!=g_LoadedModuleMap.end() )
		ret = dlclose( (it->second) );

	return (ret==0) ? false : true;
}
void findSymbol( const char* szModuleName, const char* szProcName, EXTENSIBLEAPPPROC& p, bool bLoad/*=true*/ ) {

	std::string stemp = szModuleName;
	std::map<std::string,void*>::iterator it = g_LoadedModuleMap.find( stemp );

	if( it!=g_LoadedModuleMap.end() )
		*(void**)(&p) = dlsym( (it->second), szProcName );
	else {
		if( bLoad ) {
			if( loadModule( szModuleName ) )
				findSymbol( szModuleName,szProcName,p,bLoad );
		} else
			p = NULL;
	}
}
// ************************************************************************************************

#endif
