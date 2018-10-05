#ifndef atTileH
#define atTileH
#include "atExplorerObject.h"
#include "atChannel.h"
#include "Poco/Path.h"
#include "dslSharedPointer.h"
//---------------------------------------------------------------------------

namespace at
{

using Poco::Path;
class Section;

class Tile;
typedef std::tr1::shared_ptr<Tile> TileSP;

//!A tile is a (TIFF) image (on disk), typically part of an imaged section
//A tile is associated with a section and channel, and has an id.
class ATE_DATA Tile : public ExplorerObject
{
    public:
                                    Tile(Channel& c, Section& s, int tileID, const Path& p);
        virtual                     ~Tile();

        bool                        assignPath(Path& p, bool checkIfExists = false);
        int                         getID(){return mID;}
        Path                        getPath(){return mRawDataPath;}
        const Channel&     		    getChannel(){return mChannel;}
        const Section&              getSection(){return mSection;}

    protected:
                                    //!Assign from path
        int            	            mID;

                                    //!This will point to the tiles 'file'
        Path                        mRawDataPath;

                                    //!Any tile 'belong' to a section
                                    //!and channel
        Section&                    mSection;
        Channel&                    mChannel;
};

}

#endif
