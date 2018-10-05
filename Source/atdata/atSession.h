#ifndef atSessionH
#define atSessionH
#include "atExplorerObject.h"
#include "atChannel.h"
#include "atChannels.h"
//---------------------------------------------------------------------------

namespace at
{

class ATE_DATA Session : public ExplorerObject
{
	public:
                                    Session(const string& lbl);
                                    ~Session();
        string                      getLabel() const {return mLabel;}
        bool        	            appendChannel(ChannelSP channel);

        ChannelSP                   getFirstChannel();
        ChannelSP                   getNextChannel();
        StringList                  getChannelLabels();
        bool                        operator==(const Session& s) const;

	protected:
        string                      mLabel;
        Channels                    mChannels;
};

}

#endif
