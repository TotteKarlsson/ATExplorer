#ifndef atFileH
#define atFileH
#include "Poco/Path.h"
#include <vector>
#include <string>
#include "atFileSystemObject.h"
//---------------------------------------------------------------------------

namespace at
{
using Poco::Path;
using std::vector;
using std::string;


class ATE_DATA File : public FileSystemObject
{
    public:
        File(const string &name)
        : FileSystemObject(name)
        {}
};

}
#endif
