#ifndef atExplorerObjectH
#define atExplorerObjectH
#include "atExplorerCoreExporter.h"
#include <string>
//---------------------------------------------------------------------------

namespace at
{

using std::string;
class ATE_CORE ExplorerObject
{
	public:
    							        ExplorerObject();
		virtual					        ~ExplorerObject() = 0;
        virtual string 	        		getTypeName() const;
};
}
#endif