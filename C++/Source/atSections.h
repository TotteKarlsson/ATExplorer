#ifndef atSectionsH
#define atSectionsH
#include "atATObject.h"
#include <vector>
using std::vector;

namespace at
{

class Section;
//!Sections is a container for individual AT "sections".
//!A Ribbon is a set of consecutive sections
//---------------------------------------------------------------------------
class Sections : public vector<Section*>, public ATObject
{

        public:
                                            Sections();
            virtual                         ~Sections();
            int                             count(){return this->size();}
            Section*                        getFirstSection();
            Section*                        getNextSection();
            Section*                        getPreviousSection();
            Section*                        getLastSection();
            Section*                        getSection(int sectionID);

        protected:
        	vector<Section*>::iterator  	mSectionIterator;

        private:

};


}
#endif
