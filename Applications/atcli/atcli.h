#ifndef atcliH
#define atcliH
#include "tclap/CmdLine.h"
#include <string>
//---------------------------------------------------------------------------

using std::string;
using TCLAP::ValueArg;
using TCLAP::SwitchArg;
using TCLAP::CmdLine;

class ATCli
{
    public:
    	//Command line arguments
		ValueArg<string> 	            loglevel;
        ValueArg<string> 	            data;
        SwitchArg		 	            datasummary;
        SwitchArg 		 	            consolelogs;

//        //Render stuff
//        ValueArg<string>                deletestack;
//        SwitchArg       		        deletestacks;
//
//		SwitchArg						getstacknames;
//        ValueArg<string>                owner;
//        ValueArg<string>                project;

		TCLAP::CmdLine 		            cmdLine;


        					            ATCli(int argc, const char * argv[], const string& version);


        void                            showUsage();
		int								numberofArgsSet();
};

#endif
