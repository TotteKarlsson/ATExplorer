#pragma hdrstop
#include "atcli.h"
#include "tclap/StdOutput.h"
#include <list>
//---------------------------------------------------------------------------

using namespace TCLAP;
using namespace std;

ATCli::ATCli(int argc, const char * argv[])
:                  //Flags..................        Information.............................                                    Required..  Value.. 	type
    loglevel(		"l", 		"loglevel", 		"Set loglevel, Error, Warning, Info, Debug, Debug1-5", 	 					false, 		"Info", 	"string"),
    dataroot(		"d", 		"dataroot", 		"Absolute path to root folder of input data (project) to process", 			false, 		"", 		"string"),
    datainfo(		"",			"datainfo",		 	"Return data information", 													false),
    printjson(		"j",		"json",				"Output data in JSON format", 												false),
	consolelogs(	"",			"consolelogs", 		"console logging", 															false),

    deletestack(	"", 		"deletestack", 		"Delete render stack", 	 													false, 		"", 		"string"),
    deletestacks(  	"", 		"deletestacks", 	"Delete renderstacks", 	 													false),
    getstacknames( 	"", 		"getstacknames", 	"Get stacknames", 	 														false),
    owner(			"", 		"owner", 			"Set renderproject owner", 	 												false, 		"", 		"string"),
    project(		"", 		"project", 			"Set renderproject name", 	 												false, 		"", 		"string"),

    cmdLine("Command description message", ' ', "0.5")
{
    //Have TCLAP handle any command line excveptions..
    cmdLine.setExceptionHandling(true);
    cmdLine.add(loglevel);
    cmdLine.add(dataroot);
    cmdLine.add(printjson);
    cmdLine.add(datainfo);
    cmdLine.add(consolelogs);

    cmdLine.add(getstacknames);
    cmdLine.add(deletestack);
    cmdLine.add(deletestacks);
    cmdLine.add(owner);
    cmdLine.add(project);

    cmdLine.parse( argc, argv );
}

void ATCli::showUsage()
{
    StdOutput o;
    o.usage(cmdLine);
}

int	ATCli::numberofArgsSet()
{
    list<Arg*> args = cmdLine.getArgList();
    list<Arg*>::iterator iter;
    iter = args.begin();

    int argsSet(0);
    while(iter != args.end())
    {
        if( (*iter)->isSet() )
        {
            argsSet ++;
        }
        iter++;
    }

    return argsSet;
}


