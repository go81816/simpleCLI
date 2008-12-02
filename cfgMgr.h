// cfgMgr.h: interface for the cfgMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFGMGR_H__B3C25E9E_F9C1_4087_A9E0_5D000C84AD6E__INCLUDED_)
#define AFX_CFGMGR_H__B3C25E9E_F9C1_4087_A9E0_5D000C84AD6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>

class cfgMgr
{
public:
	cfgMgr( const char* appName );
	~cfgMgr();

	const char* getAppName( void );
	const char* getParamVal( const char* key );
	std::string outputAsString( void );
	void addKeyValPair( const char* key, const char* val, bool overridable=true );

	void readIn( void );

private:
	typedef struct _cfgParamValType {
		_cfgParamValType( const char* sz, bool b=true ) {
			sval = sz;
			boverridable = b;
		}
		std::string sval;
		bool boverridable;
	};

	std::map<std::string,_cfgParamValType> m_cfgParams;
	std::string m_sAppName;

public:
	typedef std::map<std::string,_cfgParamValType>::iterator cfgMapIt;
};

#endif // !defined(AFX_CFGMGR_H__B3C25E9E_F9C1_4087_A9E0_5D000C84AD6E__INCLUDED_)
