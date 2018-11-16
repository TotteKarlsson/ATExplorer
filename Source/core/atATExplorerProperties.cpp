#pragma hdrstop
#include "atATExplorerProperties.h"
#include "dslFileUtils.h"
#include "dslLogger.h"
#include "dslException.h"
#include "dslVCLUtils.h"
//---------------------------------------------------------------------------

namespace at
{

using namespace dsl;
using namespace std;

ATExplorerProperties::ATExplorerProperties()
:
LogLevel(lAny),
DefaultRenderPythonContainer(""),
DefaultRenderServiceContainer(""),
DefaultATModulesContainer(""),
DefaultRenderServiceHost(""),
LogFileName      (""),
LogFileLocation  (""),
mIniFile         (NULL)
{

}

ATExplorerProperties::~ATExplorerProperties()
{}

void ATExplorerProperties::init(IniFile* iniFile)
{
    mIniFile = iniFile;

    shared_ptr<IniFileProperties> section =  appendNewINISection("ATExplorer");
	section->add((BaseProperty*)  &LogLevel.setup( 	                  	"LOG_LEVEL",    	                lAny));

    //Read properties
    read();
}

void ATExplorerProperties::read()
{
    //Write all properties
    for(mSectionIterator = mSections.begin(); mSectionIterator != mSections.end(); mSectionIterator++)
    {
        PropertiesSP props = (*mSectionIterator);
        if(props)
        {
            Log(lInfo) << "Writing property section: "<<props->getSectionName()<< " with " << props->count()<<" properties";
            if(!props->read())
            {
                Log(lError) << "Failed to read "<<props->getSectionName()<<" properties";
            }
        }
    }
}

void ATExplorerProperties::write()
{
    //Write all properties
    for(mSectionIterator = mSections.begin(); mSectionIterator != mSections.end(); mSectionIterator++)
    {
        PropertiesSP props = (*mSectionIterator);
        if(props)
        {
            Log(lInfo) << "Writing property section: "<<props->getSectionName()<< " with " << props->count()<<" properties";
            props->write();
        }
    }
}

bool ATExplorerProperties::saveIniFile()
{
	if(mIniFile)
    {
    	return mIniFile->save();
    }
    return false;
}

void ATExplorerProperties::append(PropertiesSP props)
{
    mSections.push_back(props);
}

BaseProperty* ATExplorerProperties::getProperty(const string& name, const string& sectionName)
{
    PropertiesSP propSection;
    if(sectionName.size())
    {
        return propSection->getProperty(name);
    }

    propSection = getFirstSection();
    while(propSection)
    {
        if(propSection->getProperty(name))
        {
            return propSection->getProperty(name);
        }
        propSection = getNextSection();
    }
    return nullptr;
}

shared_ptr<IniFileProperties> ATExplorerProperties::appendNewINISection(const string& secName)
{
	shared_ptr<IniFileProperties> p = shared_ptr<IniFileProperties>(new IniFileProperties(mIniFile, secName));
    mSections.push_back(p);
    return p;
}

IniFile* ATExplorerProperties::getIniFile()
{
    return mIniFile;
}

PropertiesSP ATExplorerProperties::getSection(const string& sec)
{
    for(mSectionIterator = mSections.begin(); mSectionIterator != mSections.end(); mSectionIterator++)
    {
        PropertiesSP props = (*mSectionIterator);
        if(props && props->getSectionName() == sec)
        {
            return props;
        }
    }
    return PropertiesSP();
}

bool ATExplorerProperties::hasChanges()
{
    for(mSectionIterator = mSections.begin(); mSectionIterator != mSections.end(); mSectionIterator++)
    {
        PropertiesSP props = (*mSectionIterator);
        if(props && props->isModified())
        {
            return true;
        }
    }
    return false;
}

PropertiesSP ATExplorerProperties::getFirstSection() const
{
    mSectionIterator = mSections.begin();
    return (*mSectionIterator);
}

PropertiesSP ATExplorerProperties::getNextSection()
{
    mSectionIterator++;
    if(mSectionIterator == mSections.end())
    {
        return PropertiesSP();
    }
    return (*mSectionIterator);
}

PropertiesSP ATExplorerProperties::getCurrentSection()
{
    return  (*mSectionIterator);
}

//Todo, move this away
string ATExplorerProperties::getLogFileNameAndPath()
{
    return "";
}

//Todo, move this away
void ATExplorerProperties::setupLogging()
{
	//Get Application folder
//	if(!folderExists(AppDataFolder))
//	{
//		createFolder(AppDataFolder);
//	}
//
//	string fullLogFileName(joinPath(AppDataFolder	, AppName + ".log"));
//	clearFile(fullLogFileName);
//
//	dsl::gLogger.logToFile(fullLogFileName);
//	LogOutput::mShowLogLevel = true;
//	LogOutput::mShowLogTime = false;
//	LogOutput::mUseLogTabs = false;
//	Log(lInfo) << "Logger was setup";
}

}
