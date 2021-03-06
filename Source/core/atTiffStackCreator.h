#ifndef atTiffStackCreatorH
#define atTiffStackCreatorH
#include "atExplorerObject.h"
#include "atTiffStackProject.h"
#include "dslProcess.h"
#include "dslSharedPointer.h"
//---------------------------------------------------------------------------
namespace at
{

using dsl::Process;
using dsl::StringList;

class ATE_CORE TiffStackCreator : public ExplorerObject
{
    public:
                                        TiffStackCreator(const string& imPath, const string& wf);
                                        ~TiffStackCreator();
        void                            setStackName(const string& name);
        void                            assignCallbacks(dsl::Callback enter, dsl::Callback progress, dsl::Callback onExit);
        void                            assignOpaqueData(void* arg1, void* arg2);

        shared_ptr<TiffStackProject>           create(const StringList& fileNames, const string& outputFileName);
        bool                            checkForImageMagick();
        bool                            setOutputFolder(const string& path);
        shared_ptr<TiffStackProject>           getStack();

    protected:
        string                          mImageMagickPath;
        string                          mOutputFolder;
        string                          mConvertExe;
        shared_ptr<TiffStackProject>           mTheStack;
	    Process 						mTheProcess;
		dsl::Callback 					mOnEnterCB;
        dsl::Callback 					mOnProgressCB;
        dsl::Callback 					mOnExitCB;


//        void                            onIMProcessFinished(void*, void*);
};

}

#endif
