#ifndef atChannelsH
#define atChannelsH
#include "atExplorerObject.h"
#include "atChannel.h"
#include "dslStringList.h"
#include <vector>
//---------------------------------------------------------------------------

namespace at
{

using std::vector;
using dsl::StringList;

typedef shared_ptr<Channel> ChannelSP;

//!The Channels class is a container for AT Channels
class ATE_DATA Channels : public ExplorerObject
{
    public:
                                                Channels();
                                                ~Channels();

        bool                                    append(ChannelSP ch);
        bool                                    append(const Channels& chns);
        int                                     count();
        ChannelSP                               getFirstChannel();
        ChannelSP                               getNextChannel();
		ChannelSP                               getChannel(ChannelSP c);
		ChannelSP                               getChannel(const string& chLbl);
        StringList                              asStringList();

    protected:
        vector<ChannelSP>::iterator             mChannelIterator;
        vector<ChannelSP>                       mChannels;
};

}
#endif
