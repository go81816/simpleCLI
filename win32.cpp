#if defined(WIN32)

#include "simpleCLI_platformspec.h"

#include <string>
#include <map>

// ************************************************************************************************
// **********************************Windows Registry Functions************************************
// ************************************************************************************************
#define MAX_VALUE_NAME 32767

int getRegOption( LPCTSTR szApp, LPCTSTR szValName, std::string& sVal ) {
	HKEY hkey = HKEY_CURRENT_USER;
	DWORD options = 0 /* reserved, must be zero */;
	REGSAM sam = KEY_READ;
	HKEY hkeyresult;
	LONG lRet = RegOpenKeyEx( hkey,szApp,options,sam,&hkeyresult );
	if( lRet!=ERROR_SUCCESS ) {
		hkey = HKEY_LOCAL_MACHINE; // try local machine hive
		lRet = RegOpenKeyEx( hkey,szApp,options,sam,&hkeyresult );
	}
	if( lRet!=ERROR_SUCCESS )
		return 0;  //bow out early if we can't even open the app key in one of the hives

	DWORD* pResvd = NULL;
	DWORD type;
	DWORD val_size;
	lRet = ERROR_SUCCESS;
	if( lRet==ERROR_SUCCESS ) {
		lRet = RegQueryValueEx( hkeyresult,szValName,pResvd,&type,NULL,&val_size );
		if( lRet!=ERROR_SUCCESS ) {
			RegCloseKey( hkeyresult );
			return 0;
		}

		LPBYTE val = (LPBYTE)malloc( val_size );
		lRet = RegQueryValueEx( hkeyresult,szValName,pResvd,&type,val,&val_size );
		if( lRet==ERROR_SUCCESS ) {
			switch( type ) {
			case REG_SZ:
				// null terminator may not be there, set it
				val[val_size] = '\0';
				sVal = (char*) val;
				break;
			case REG_BINARY:
//				break;
			case REG_DWORD:
//			case REG_DWORD_LITTLE_ENDIAN:
//				break;
			case REG_DWORD_BIG_ENDIAN:
//				break;
			case REG_QWORD:
//			case REG_QWORD_LITTLE_ENDIAN:
//				break;

			case REG_NONE:
//				break;
			case REG_MULTI_SZ:
//				break;
			case REG_LINK:
//				break;
			case REG_EXPAND_SZ:
//				break;
			default:
				//type not supported, march onto next key
				;
			}
			free( val );
		} else {
			RegCloseKey( hkeyresult );
			return 0;
		}
	}

	RegCloseKey( hkeyresult );
	return 1;
}

int getRegOption( LPCTSTR szApp, DWORD idx, std::string& sKey, std::string& sVal ) {

	HKEY hkey = HKEY_CURRENT_USER;
	DWORD options = 0 /* reserved, must be zero */;
	REGSAM sam = KEY_READ;
	HKEY hkeyresult;
	LONG lRet = RegOpenKeyEx( hkey,szApp,options,sam,&hkeyresult );
	if( lRet!=ERROR_SUCCESS ) {
		hkey = HKEY_LOCAL_MACHINE; // try local machine hive
		lRet = RegOpenKeyEx( hkey,szApp,options,sam,&hkeyresult );
	}
	if( lRet!=ERROR_SUCCESS )
		return 0;  //bow out early if we can't even open the app key in one of the hives

	TCHAR valname[MAX_VALUE_NAME];
	DWORD valname_size = MAX_VALUE_NAME;
	DWORD* pResvd = NULL;
	DWORD type;
	DWORD val_size;
	lRet = ERROR_SUCCESS;
	if( lRet==ERROR_SUCCESS ) {
		lRet = RegEnumValue( hkeyresult,idx,valname,&valname_size,pResvd,&type,NULL,&val_size );
		if( lRet!=ERROR_SUCCESS ) {
			RegCloseKey( hkeyresult );
			return 0;
		}

		LPBYTE val = (LPBYTE)malloc( val_size );
		lRet = RegEnumValue( hkeyresult,idx,valname,&valname_size,pResvd,&type,val,&val_size );
		if( lRet==ERROR_SUCCESS ) {
			sKey = valname;
			switch( type ) {
			case REG_SZ:
				// null terminator may not be there, set it
				val[val_size] = '\0';
				sVal = (char*) val;
				break;
			case REG_BINARY:
//				break;
			case REG_DWORD:
//			case REG_DWORD_LITTLE_ENDIAN:
//				break;
			case REG_DWORD_BIG_ENDIAN:
//				break;
			case REG_QWORD:
//			case REG_QWORD_LITTLE_ENDIAN:
//				break;

			case REG_NONE:
//				break;
			case REG_MULTI_SZ:
//				break;
			case REG_LINK:
//				break;
			case REG_EXPAND_SZ:
//				break;
			default:
				//type not supported, march onto next key
				;
			}
			free( val );
		} else {
			RegCloseKey( hkeyresult );
			return 0;
		}
	}

	RegCloseKey( hkeyresult );
	return 1;
}
// ************************************************************************************************

// ************************************************************************************************
// **********************************Dynamic Loading Functions*************************************
// ************************************************************************************************
static std::map<std::string,HMODULE> g_LoadedModuleMap;
bool loadModule( LPCTSTR szModuleName ) {

	HANDLE hResvd = NULL;
	DWORD dwFlags = 0;
	HMODULE hMod = LoadLibraryEx( szModuleName, hResvd, dwFlags );

	bool bRet = false;
	if( hMod!=NULL ) {
		std::string sModName = szModuleName;
		std::pair<std::map<std::string,HMODULE>::iterator,bool > pr;
		pr = g_LoadedModuleMap.insert( std::pair<std::string,HMODULE>(sModName,hMod) );
		bRet = pr.second;
	}

	return bRet;
}
bool unloadModule( LPCTSTR szModuleName ) {

	std::string stemp = szModuleName;
	std::map<std::string,HMODULE>::iterator it = g_LoadedModuleMap.find( stemp );

	BOOL ret = 0;   //if a module is not found, should this return true or false?
	if( it!=g_LoadedModuleMap.end() )
		ret = FreeLibrary( (it->second) );

	return (ret==0) ? false : true;
}
void findSymbol( LPCTSTR szModuleName, LPCSTR szProcName, EXTENSIBLEAPPPROC& p, bool bLoad/*=true*/ ) {

	std::string stemp = szModuleName;
	std::map<std::string,HMODULE>::iterator it = g_LoadedModuleMap.find( stemp );

	if( it!=g_LoadedModuleMap.end() )
		p = (EXTENSIBLEAPPPROC)GetProcAddress( (it->second), szProcName );
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