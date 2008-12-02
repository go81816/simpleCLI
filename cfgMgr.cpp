// cfgMgr.cpp: implementation of the cfgMgr class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4786) 
#include "cfgMgr.h"

#include "simpleCLI_platformspec.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cfgMgr::cfgMgr( const char* appName ) : m_sAppName( appName ) {}

cfgMgr::~cfgMgr() {}

const char* cfgMgr::getAppName( void ) {
	return m_sAppName.c_str();
}

const char* cfgMgr::getParamVal( const char* key ) {
	std::string stemp = key;
	cfgMgr::cfgMapIt it = m_cfgParams.find( stemp );
	if( it!=m_cfgParams.end() ) 
		return (it->second).sval.c_str();
	else
		return NULL;
}

std::string cfgMgr::outputAsString( void ) {
	std::string sRetVal = "";
	cfgMgr::cfgMapIt it = m_cfgParams.begin();
	while( it!=m_cfgParams.end() ) {
		sRetVal += ((it)->first);
		sRetVal += "=";
		if( !((it)->second).boverridable )
			sRetVal += "~";
		sRetVal += ((it)->second).sval;
		sRetVal += ";";
		it++;
	}
	return sRetVal.c_str();
}

void cfgMgr::addKeyValPair( const char* key, const char* val, bool overridable/*=true*/ ) {
	cfgMgr::_cfgParamValType newparam(val);
	std::string stemp = key;
	std::pair<cfgMgr::cfgMapIt,bool > pr;
	pr = m_cfgParams.insert( std::pair<std::string,cfgMgr::_cfgParamValType>(stemp,newparam) );
	// the only thing special about this function is it forces overrides of key values
	// when the key already exists and the key hasnt been overriden more than max_override times
	// default is to always override
	//  (this is useful when command line options are specified, but you want to reread app
	//    options from a configuration file so that they can change, but don't change options
	//    specified on the command line)
	if( pr.second==false ) {
		if( (((pr.first)->second).boverridable) ) {
			((pr.first)->second).sval=val;
			((pr.first)->second).boverridable = overridable;
		}
	}
}

void cfgMgr::readIn( void ) {
	std::string sKey;
	std::string sVal;
	std::string sAppName = getAppName();

/* 
 * There may be a better way to structure this.
 * My concept is to have platform dependent "loop/starter" sections
 * followed by some consistent platform independent "actual work" and
 * then a platform dependent "loop/cleanup" section
 * This way the actual work is consistent across platforms
 */
#if defined(WIN32)
	DWORD idx=0;
	while( getRegOption( sAppName.c_str(),idx++,sKey,sVal ) ) {
#elif defined(MACOS_X_UNIX)
	// where does mac store app cfg info ?
#elif defined(LINUX)
	// is there "standard" code used under linux for /etc/app/*.conf file processing?
	//  i'm thinking like POSIX-type routines?
#else
	//  for now just do basic file i/o
//	std::string scfgFileName = sAppName.c_str() + ".conf";
	std::string scfgFileName(sAppName.c_str());
	scfgFileName.append(".conf");
	FILE* fCfg = fopen( scfgFileName.c_str(), "r" );
	if( fCfg==NULL )
		return;  // bow out early if can't even open config file

	const int MAX_CONF_FILE_LINE_LENGTH = 2048;
	char buf[MAX_CONF_FILE_LINE_LENGTH];
	while( fgets(buf,MAX_CONF_FILE_LINE_LENGTH,fCfg)!=NULL ) {
		//for now, just treat as key=val pair, 1 per line
		if( buf[0]=='#' ) continue;
		if( !strlen(buf) ) continue;

		std::string sLine( buf );
		std::string::size_type pos=sLine.find( "=" );
		if( pos==std::string::npos ) continue;

		sKey = sLine.substr(0,pos);
		sVal = sLine.substr(pos+1);
#endif


			addKeyValPair( sKey.c_str(),sVal.c_str() );


#if defined(WIN32)
	}
#elif defined(MACOS_X_UNIX)
	//
#elif defined(LINUX)
	//
#else
	}
	fclose( fCfg );
#endif
}
