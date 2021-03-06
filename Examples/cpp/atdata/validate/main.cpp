#pragma hdrstop
#pragma argsused
#include "atATIFData.h"
#include "dslLogger.h"
#include "atATEExceptions.h"
#include "atSession.h"
#include "atRibbon.h"

using namespace dsl;
using namespace at;

//Arg1 -> AtData
void onStartingPopulating( void* arg1, void* arg2, void* arg3)
{
    Log(lInfo) << "Starting populating data..";
}

//Arg1 -> AtData
void onProgressPopulating(void* arg1, void* arg2, void* arg3)
{
    ATData* data = (ATData*)arg1;
    Log(lInfo) << "Progressing populating data: ";
    if(data)
    {
        Log(lInfo) << "Sections: " <<data->getNumberOfSections();
    }
}

//Arg1 -> AtData
void onFinishedPopulating(void* arg1, void* arg2, void* arg3)
{
    ATData* data = (ATData*)arg1;

    Log(lInfo) << "Finished populating data";
   	Log(lInfo) << data->getInfo();
}

int main()
{
    //Setup logging
    LogOutput::mShowLogLevel = true;
    gLogger.setLogLevel(lInfo);
    gLogger.logToConsole(true);
    gLogger.logToFile("p:\\ATProjects\\MyLog.txt");

    Path dataPath("c:\\data\\M33\\");

	ATData* atData(nullptr);

    for(int i = 0; i < 100; i++)
    {
        if(atData)
        {
            delete atData;
        }
        atData = new ATIFData(dataPath);
	    atData->assignOnPopulateCallbacks(onStartingPopulating, onProgressPopulating, onFinishedPopulating);
        try
        {
            //!Populating the data object causes a scan of folders and files
            //!representing the data. No image data is loaded
            atData->populate(false);

            //Print some information about ribbons and sections
            Log(lInfo) << "This is a "<<atData->getNumberOfRibbons()<<" ribbon(s) dataset";

            RibbonSP ribbon = atData->getFirstRibbon();
            while(ribbon)
            {
                Log(lInfo) << "There are "<<ribbon->getNumberOfSections()<<" sections in ribbon "<< ribbon->getAlias();
                ribbon = atData->getNextRibbon();
            }

            //Check Sessions and channels, i.e. actual data
            SessionSP session =  atData->getFirstSession();
            while(session)
            {
                Log(lInfo) << "Checking session " << session->getLabel();

                //Get Channels in session
                ChannelSP ch = session->getFirstChannel();
                while(ch)
                {
                    Log(lInfo) << "Checking channel: " << ch->getLabel();
                    RibbonSP ribbon = atData->getFirstRibbon();
                    while(ribbon)
                    {
                        SectionSP section = ribbon->getFirstSection();
                        //Loop through sections
                        while(section)
                        {
                            TilesSP tiles = section->getTiles(ch);
                            if(tiles)
                            {
    	                        Log(lDebug) << "There are " << tiles->count() << " tiles in section: " << section->id()<< ", channel \"" << tiles->getChannel().getLabel() << "\" in session: "<<session->getLabel()<<" on ribbon \"" << ribbon->getAlias() << "\"";
                            }
                            section = ribbon->getNextSection();
                        }

                        ribbon = atData->getNextRibbon();
                    }
                    ch = session->getNextChannel();
                }
                session = atData->getNextSession();
            }

            //Total number of tiles??
            long nrOfTiles = atData->getNumberOfTiles();
            Log(lInfo) << "Number of Tiles: " << nrOfTiles;
        }
        catch(const FileSystemException& e)
        {
            Log(lError) << "Exception: " << e.what();
            Log(lInfo) << "Application exiting..";
        }
        catch(...)
        {

        }
    }
	return 0;
}

#pragma comment(lib, "atExplorerFoundation")
#pragma comment(lib, "dslFoundation")
#pragma comment(lib, "poco_foundation")
#pragma comment(lib, "tinyxml2")
