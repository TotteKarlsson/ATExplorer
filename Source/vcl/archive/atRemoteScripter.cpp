#include <vcl.h>
#pragma hdrstop
#include "atRemoteScripter.h"
#include "dslVCLUtils.h"
#include "dslLogger.h"
#include <sstream>
#pragma package(smart_init)
//---------------------------------------------------------------------------

namespace at
{

using namespace std;
using namespace dsl;
using namespace Scsshchannel;

RemoteScripter::RemoteScripter(const string& sn, TScSSHShell* ssh)
:
mScriptName(sn),
mSSHShell(ssh)
{}

RemoteScripter::~RemoteScripter()
{}

string RemoteScripter::getRemoteCommand(){return mRemoteCommand;}
void   RemoteScripter::useConnection(TScSSHShell* c){mSSHShell = c;}

void RemoteScripter::clearArguments()
{
    mArguments.clear();
}

void RemoteScripter::addArgument(const string& argument)
{
    mArguments.append(argument);
}

StringList RemoteScripter::getArguments()
{
    return mArguments;
}

void RemoteScripter::setRemoteCommand(const string& cmd)
{
    mRemoteCommand = cmd;
}

string RemoteScripter::createFullRemoteCommand()
{
    stringstream cmd;
    cmd << mRemoteCommand << " " << mScriptName;
    for(uint i = 0; i < mArguments.count(); i++)
    {
        cmd <<" " << mArguments[i];
    }

    cmd << endl;
    return cmd.str();
}

void RemoteScripter::run()
{
    string cmd = createFullRemoteCommand();
    Log(lDebug) << "Running remote command: " << cmd;
    mServerResponse.clear();
	mSSHShell->WriteString(vclstr(cmd));
}
}

