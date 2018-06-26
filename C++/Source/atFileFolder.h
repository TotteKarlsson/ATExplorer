#ifndef atFileFolderH
#define atFileFolderH
#include "Poco/Path.h"
#include <set>
#include <string>
#include "atFileSystemObject.h"
#include "dslStringList.h"
#include "atFileFolders.h"
#include <utility>
//---------------------------------------------------------------------------

namespace at
{

using std::pair;
using std::vector;
using std::set;
using std::string;
using Poco::Path;
using dsl::StringList;

typedef pair<int, int> FolderInfo;

//!A File folder is FileSystem object.
class FileFolder : public FileSystemObject
{
    public:
                            		FileFolder(const Path& name, FileFolder* parent = NULL);
                            		~FileFolder();
        FileFolder*                 getFirstSubFolder();
        FileFolder*                 getNextSubFolder();

        FileFolder*   				getSubFolder(const Path& p);
        FileFolders         		getSubFolders(const Path& subPath = Path("."));
        const set<string>&          getFiles(const string& globPattern = "*.*");
        StringList          		getSubFoldersAsList();
        bool                        isPresent(FileSystemObject* child);
        void 				        addSubFolder(FileFolder* child);
        void 				        removeChild(FileSystemObject* child);
        string						getLastPartOfPath();
        string                      directoryName(int n);
        FolderInfo                	scan();

    private:
	    FileFolders					mSubFolders;
        set<string>                 mFiles;
};


}


#endif
