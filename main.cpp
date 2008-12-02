// This is a bit like a cross-platform rundll32 concept
//  with a dangerous trend towards instituting too much 
//  shell like functionality

/*
 * Things to add:
 *     standard command-line parsing  -- getopt port or boost?
 *     Mac support ?
 */

#pragma warning (disable : 4786) 

#include <string>
#include <iostream>

#include "simpleCLI_platformspec.h"

#include "cfgMgr.h"

// app understood option defaults...
#define DEFAULT_INPUT_PROMPT "input?"
#define DEFAULT_MAX_COMMAND_LENGTH "1024"
#define DEFAULT_MODULE_OVERRIDE "-mod_path="
#define DEFAULT_CMD_PARAM_DELIM " "
#define DEFAULT_REREAD_OPTIONS "0"
#define DEFAULT_ONESHOT_MODE "0"
#define DEFAULT_INTERNAL_COMMAND_TEXT ".internal"
#define DEFAULT_INTERNAL_EXIT_COMMAND_OPTION "exit"
#define DEFAULT_INTERNAL_REREAD_OPTIONS_COMMAND_OPTION "reload_options"
#define DEFAULT_INTERNAL_SET_CFGPARAMVAL_COMMAND_OPTION "set_cfgparam"
#define DEFAULT_INTERNAL_GET_CFGPARAMVALS_COMMAND_OPTION "get_cfgparams"
//...names
#define INPUT_PROMPT_OPTION "INPUT_PROMPT"
#define MAX_COMMAND_LENGTH_OPTION "MAX_COMMAND_LENGTH"
#define MODULE_OVERRIDE_OPTION "MODULE_OVERRIDE"
#define CMD_PARAM_DELIMITER_OPTION "CMD_PARAM_DELIMITER"
#define REREAD_OPTIONS_OPTION "REREAD_OPTIONS"
#define ONESHOT_MODE_OPTION "ONESHOT_MODE"
#define INTERNAL_COMMAND_TEXT_OPTION "INTERNAL_COMMAND_TEXT"
#define INTERNAL_EXIT_COMMAND_OPTION_OPTION "INTERNAL_EXIT_COMMAND_OPTION"
#define INTERNAL_REREAD_OPTIONS_COMMAND_OPTION_OPTION "INTERNAL_REREAD_OPTIONS_COMMAND_OPTION"
#define INTERNAL_SET_CFGPARAMVAL_COMMAND_OPTION_OPTION "INTERNAL_SET_CFGPARAMVAL_COMMAND_OPTION"
#define INTERNAL_GET_CFGPARAMVALS_COMMAND_OPTION_OPTION "INTERNAL_GET_CFGPARAMVALS_COMMAND_OPTION"

int main( int argc, char** argv ) {

	cfgMgr appcfg("simpleCLI");
	// add default values first
	appcfg.addKeyValPair( INPUT_PROMPT_OPTION, ((getenv(INPUT_PROMPT_OPTION)==NULL) || strlen(getenv(INPUT_PROMPT_OPTION))==0) ? DEFAULT_INPUT_PROMPT : getenv(INPUT_PROMPT_OPTION) );
	appcfg.addKeyValPair( MAX_COMMAND_LENGTH_OPTION, ((getenv(MAX_COMMAND_LENGTH_OPTION)==NULL) || strlen(getenv(MAX_COMMAND_LENGTH_OPTION))==0) ? DEFAULT_MAX_COMMAND_LENGTH : getenv(MAX_COMMAND_LENGTH_OPTION) );
	appcfg.addKeyValPair( MODULE_OVERRIDE_OPTION, ((getenv(MODULE_OVERRIDE_OPTION)==NULL) || strlen(getenv(MODULE_OVERRIDE_OPTION))==0) ? DEFAULT_MODULE_OVERRIDE : getenv(MODULE_OVERRIDE_OPTION) );
	appcfg.addKeyValPair( CMD_PARAM_DELIMITER_OPTION, ((getenv(CMD_PARAM_DELIMITER_OPTION)==NULL) || strlen(getenv(CMD_PARAM_DELIMITER_OPTION))==0) ? DEFAULT_CMD_PARAM_DELIM : getenv(CMD_PARAM_DELIMITER_OPTION) );
	appcfg.addKeyValPair( REREAD_OPTIONS_OPTION, ((getenv(REREAD_OPTIONS_OPTION)==NULL) || strlen(getenv(REREAD_OPTIONS_OPTION))==0) ? DEFAULT_REREAD_OPTIONS : getenv(REREAD_OPTIONS_OPTION) );
	appcfg.addKeyValPair( ONESHOT_MODE_OPTION, ((getenv(ONESHOT_MODE_OPTION)==NULL) || strlen(getenv(ONESHOT_MODE_OPTION))==0) ? DEFAULT_ONESHOT_MODE : getenv(ONESHOT_MODE_OPTION) );
	appcfg.addKeyValPair( INTERNAL_COMMAND_TEXT_OPTION, ((getenv(INTERNAL_COMMAND_TEXT_OPTION)==NULL) || strlen(getenv(INTERNAL_COMMAND_TEXT_OPTION))==0) ? DEFAULT_INTERNAL_COMMAND_TEXT : getenv(INTERNAL_COMMAND_TEXT_OPTION) );
	appcfg.addKeyValPair( INTERNAL_EXIT_COMMAND_OPTION_OPTION, ((getenv(INTERNAL_EXIT_COMMAND_OPTION_OPTION)==NULL) || strlen(getenv(INTERNAL_EXIT_COMMAND_OPTION_OPTION))==0) ? DEFAULT_INTERNAL_EXIT_COMMAND_OPTION : getenv(INTERNAL_EXIT_COMMAND_OPTION_OPTION) );
	appcfg.addKeyValPair( INTERNAL_REREAD_OPTIONS_COMMAND_OPTION_OPTION, ((getenv(INTERNAL_REREAD_OPTIONS_COMMAND_OPTION_OPTION)==NULL) || strlen(getenv(INTERNAL_REREAD_OPTIONS_COMMAND_OPTION_OPTION))==0) ? DEFAULT_INTERNAL_REREAD_OPTIONS_COMMAND_OPTION : getenv(INTERNAL_REREAD_OPTIONS_COMMAND_OPTION_OPTION) );
	appcfg.addKeyValPair( INTERNAL_SET_CFGPARAMVAL_COMMAND_OPTION_OPTION, ((getenv(INTERNAL_SET_CFGPARAMVAL_COMMAND_OPTION_OPTION)==NULL) || strlen(getenv(INTERNAL_SET_CFGPARAMVAL_COMMAND_OPTION_OPTION))==0) ? DEFAULT_INTERNAL_SET_CFGPARAMVAL_COMMAND_OPTION : getenv(INTERNAL_SET_CFGPARAMVAL_COMMAND_OPTION_OPTION) );
	appcfg.addKeyValPair( INTERNAL_GET_CFGPARAMVALS_COMMAND_OPTION_OPTION, ((getenv(INTERNAL_GET_CFGPARAMVALS_COMMAND_OPTION_OPTION)==NULL) || strlen(getenv(INTERNAL_GET_CFGPARAMVALS_COMMAND_OPTION_OPTION))==0) ? DEFAULT_INTERNAL_GET_CFGPARAMVALS_COMMAND_OPTION : getenv(INTERNAL_GET_CFGPARAMVALS_COMMAND_OPTION_OPTION) );

	/* 
		should also parse these from the command line 
	 */
	//appcfg.addKeyValPair( MAX_COMMAND_LENGTH_OPTION, "1023",false );   //set overridable flag to false for these
	
	// read in from *where ever*
	appcfg.readIn();  

	std::string full_cmd( "dummy-init" );
	char* szFullCmd = NULL;

	std::string sRereadOptions = appcfg.getParamVal(REREAD_OPTIONS_OPTION);
	bool bRereadOpt = (::atoi( sRereadOptions.c_str() )==0) ? false : true;
	std::string sOneshotMode = appcfg.getParamVal(ONESHOT_MODE_OPTION);
	bool bOneshotMode = (::atoi( sOneshotMode.c_str() )==0) ? false : true;
	while( 1==1 ) {
		if( bRereadOpt )
			appcfg.readIn(); 

		int iMaxCmdLen = ::atoi(appcfg.getParamVal(MAX_COMMAND_LENGTH_OPTION));
		szFullCmd = (char*)malloc( iMaxCmdLen );  // alloc new buffer everytime, so that it can change on the fly
		std::cout << appcfg.getParamVal( INPUT_PROMPT_OPTION ) << appcfg.getParamVal( CMD_PARAM_DELIMITER_OPTION );
		std::cin.getline( szFullCmd, iMaxCmdLen-1 );
		full_cmd.assign( szFullCmd );
		free( szFullCmd  );

		// breakup input into cmd followed by cmdparams (separated by configurable delimiter)
		std::string sCmdParamDelim = appcfg.getParamVal( CMD_PARAM_DELIMITER_OPTION );
		std::string sCmd = "";
		std::string sCmdParams = "";
		std::string::size_type spacepos = full_cmd.find( sCmdParamDelim.c_str() );
		if( spacepos!=std::string::npos ) {
			sCmd = full_cmd.substr(0,spacepos);
			sCmdParams = full_cmd.substr(spacepos+1);
		} else 
			sCmd = full_cmd;
//std::cout << "DEBUG: " << std::endl << "cmd = " << sCmd << std::endl << "cmdparams = " << sCmdParams << std::endl;

		// only cmd option automatically sought by this prog is the module override option
		//  so that similar commands can be defined in a common module, and you can continue
		//  to work with other modules in this session
		std::string sModuleOverrideText = appcfg.getParamVal( MODULE_OVERRIDE_OPTION );
		std::string sModuleNameOverride = "";
		std::string::size_type modpos = sCmdParams.find( sModuleOverrideText.c_str() );
		if( modpos!=std::string::npos ) {
			std::string::size_type valstart = modpos+sModuleOverrideText.length();
			std::string::size_type nextspacepos = sCmdParams.find( sCmdParamDelim.c_str(),valstart );
			sModuleNameOverride = sCmdParams.substr(valstart,nextspacepos-valstart);

			//  (should this be removed then from the supplied params?)
			std::string temp = sCmdParams.substr(0,modpos) + sCmdParams.substr(nextspacepos+1);
			sCmdParams = temp;
		}

		std::string sInternalCmdText = appcfg.getParamVal( INTERNAL_COMMAND_TEXT_OPTION );

		EXTENSIBLEAPPPROC external_sub = NULL;
		std::string sModule = sCmd + DYNAMIC_MODULE_EXT;
		if( sModuleNameOverride.length()!=0 ) {
			if( sModuleNameOverride.find(DYNAMIC_MODULE_EXT)!=std::string::npos )
				sModule = sModuleNameOverride;
			else
				sModule = sModuleNameOverride + DYNAMIC_MODULE_EXT;
		}
//std::cout << "DEBUG: " << std::endl << "module = " << sModule << std::endl;
		findSymbol( sModule.c_str(), sCmd.c_str(), external_sub );
		if( external_sub!=NULL ) {
			int external_return = (external_sub) (appcfg.outputAsString().c_str(), sCmdParams.c_str());
			/* potentially could setup automatic responses to certain external_returns */
		} else if( sCmd.compare( sInternalCmdText.c_str() )==0 ) {  //could provide internal commands for complete control of this app

			std::string sInternalExitCmdOption = appcfg.getParamVal( INTERNAL_EXIT_COMMAND_OPTION_OPTION );
			std::string sInternalRereadOptionsCmdOption = appcfg.getParamVal( INTERNAL_REREAD_OPTIONS_COMMAND_OPTION_OPTION );
			std::string sInternalGetCfgParamValsCmdOption = appcfg.getParamVal( INTERNAL_GET_CFGPARAMVALS_COMMAND_OPTION_OPTION );
			std::string sInternalSetCfgParamValCmdOption = appcfg.getParamVal( INTERNAL_SET_CFGPARAMVAL_COMMAND_OPTION_OPTION );
			if( sCmdParams.compare( sInternalExitCmdOption.c_str() )==0 ) {  // sCmdParams tell what to do specifically
				break;  //exit out of main program loop
			} else if( sCmdParams.compare( sInternalRereadOptionsCmdOption.c_str() )==0 ) {
				appcfg.readIn();   //reread app cfg parameters from file/registry
			} else if( sCmdParams.compare( sInternalGetCfgParamValsCmdOption.c_str() )==0 ) {
				std::cout << appcfg.outputAsString().c_str();
			} else if( sCmdParams.substr(0,sInternalSetCfgParamValCmdOption.length()).compare( sInternalSetCfgParamValCmdOption.c_str() )==0 ) {
				std::string::size_type keyvalstart = sCmdParams.find( sCmdParamDelim.c_str() );
				std::string::size_type equalspos = sCmdParams.find( "=",keyvalstart+1 );
				if( (keyvalstart==std::string::npos)||(equalspos==std::string::npos) ) {
					std::cout << "unknown internal set_cfgparamval option format" << std::endl;
				} else {
					std::string sSpecKey = sCmdParams.substr( keyvalstart+1,equalspos-(keyvalstart+1) );
					std::string::size_type nextoptstart = sCmdParams.find( sCmdParamDelim.c_str(),keyvalstart+1 );
					std::string sSpecVal = "";
					bool bSpecOverridable = true;
					if( nextoptstart==std::string::npos )
						sSpecVal = sCmdParams.substr(equalspos+1);
					else {
						sSpecVal = sCmdParams.substr(equalspos+1,nextoptstart-(equalspos+1));
						bSpecOverridable = (::atoi(sCmdParams.substr(nextoptstart+1).c_str())==0) ? false : true;
					}
					appcfg.addKeyValPair( sSpecKey.c_str(),sSpecVal.c_str(),bSpecOverridable );
				}
			} else {
				std::cout << "unsupported internal command option(s): " << sCmdParams << std::endl;
			}

		} else {
			//unsupported command
			std::cout << "unsupported: " << sCmd << std::endl;
		}

		std::cout << std::endl;
		if( bOneshotMode )
			break;
	}

	return 0;
}

