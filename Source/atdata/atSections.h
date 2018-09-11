#ifndef atSectionsH
#define atSectionsH
#include <vector>
#include "atExplorerObject.h"
#include "atATDataExporter.h"

using std::vector;

namespace ate
{

class Section;
//!Sections is a container for individual AT "sections".
//!A Ribbon is a set of consecutive sections
//---------------------------------------------------------------------------
class ATE_DATA Sections : public vector<Section*>, public ExplorerObject
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