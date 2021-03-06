#ifndef atSectionH
#define atSectionH
#include "atExplorerObject.h"
#include "atATDataExporter.h"
#include "atChannel.h"
#include "atTile.h"
#include "atTiles.h"
#include <vector>
//---------------------------------------------------------------------------

namespace at
{

using std::vector;
using std::map;
class Ribbon;
class Tiles;
class Section;

typedef shared_ptr<Tiles> TilesSP;
typedef shared_ptr<Section> SectionSP;

//!An atSection is part of a ribbon.
class ATE_DATA Section : public ExplorerObject
{
    public:
                                                    Section(int id, const Ribbon& r);
        virtual                                     ~Section();
        virtual string                        		getTypeName() const;
        int                                         id() const{return mID;}
        void                                        addTile(TileSP tile);
        TilesSP                                     getTiles(const ChannelSP& ch);
        int                                         getTotalNumberOfTiles();
        int                                         getNumberOfTiles(const ChannelSP& ch);


    protected:
        int                                         mID;
        const Ribbon&                               mRibbon;

                                                    //Each Tiles container hold tilesby channel
        vector<TilesSP>                              mTiles;
};

}


#endif
