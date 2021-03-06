#pragma hdrstop
#include "TRenderProjectFrame.h"
#include "dslVCLUtils.h"
#include "atRenderServiceParameters.h"
#include "dslLogger.h"
#include "dslFileUtils.h"
#include <gdiplus.h>
#include "Poco/Path.h"
#include "boost/filesystem.hpp"
#include "TImageForm.h"
#include "atRenderLayer.h"
#include "TCreateLocalVolumesForm.h"
#include "atVCLUtils2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "dslTPropertyCheckBox"
#pragma link "dslTSTDStringLabeledEdit"
#pragma link "dslTFloatLabeledEdit"
#pragma link "dslTIntegerLabeledEdit"
#pragma link "dslTIntegerEdit"
#pragma link "dslTSTDStringEdit"
#pragma link "RzSpnEdt"
#pragma link "dslTSTDStringLabeledEdit"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

TRenderProjectFrame *RenderProjectFrame;
using namespace dsl;
using namespace at;
using namespace Poco;

int rpFrameNr(0);

TPoint controlToImage(const TPoint& p, double scale, double stretchFactor);
//---------------------------------------------------------------------------
__fastcall TRenderProjectFrame::TRenderProjectFrame(ATExplorer& e, RenderProject& rp, TComponent* Owner)
	: TFrame(Owner),
	mExplorer(e),
    mRP(rp),
    mRC(),
    mRenderEnabled(false),
    mCreateCacheThread(rp, e.Cache),
   	mCurrentROI(mRP.getRegionOfInterest()),
    mIsDrawing(false),
    mIMPath(e.getImageMagickPath()),
    mImageGrid(Image1, PaintBox1->Canvas),
    mCreateVolumesForm(NULL),
	mTiffStackCreator(e.getImageMagickPath(), "")
{
    this->Name = string("RPFrame_" +  dsl::toString(rpFrameNr++)).c_str();
//    populate();
    mCurrentROI.assignOnChangeCallback(onROIChanged);
	mCreateCacheThread.assignCallBacks(NULL, fetchImagesOnProgress, NULL);
}

void TRenderProjectFrame::populate()
{
    int secsWidth(0);
    for(int i = 0; i < HeaderControl1->Sections->Count; i++)
    {
		secsWidth += HeaderControl1->Sections->Items[i]->Width;
    }

    TPoint p1(HeaderControl1->Left, HeaderControl1->Top);
	TPoint p2 = HeaderControl1->ClientToScreen(p1);
	mZoomFactor->Left = p1.X + secsWidth;
	mZoomFactor->Top = p1.Y;
    mZoomFactor->Parent = HeaderControl1;

    //Check cache
    RenderHostE->setValue(mRP.getRenderServiceParameters().getHost());
    OwnerE->setValue(mRP.getProjectOwner());
    ProjectE->setValue(mRP.getRenderProjectName());

    mRC.setRenderServiceParameters(mRP.getRenderServiceParameters());

    //Min and max intensity
    MinIntensityE->setReference(mRP.getMinIntensity());
    MaxIntensityE->setReference(mRP.getMaxIntensity());

    //Get stacks for project
    StringList stacks = mRC.StackDataAPI.getStacksForProject(mRP.getProjectOwner(), mRP.getRenderProjectName());
    if(!stacks.size())
    {
        return;
    }

    StackCB->ItemIndex = populateDropDown(stacks, StackCB, mRP.getSelectedStackName());
    StackCBChange(NULL);

    //Check selected channel
    string ch = mRP.getSelectedChannelName();
    if(checkItem(ChannelsCB, ch, true))
    {
        mZs->ItemIndex = mZs->Items->IndexOf(dsl::toString(mRP.getSelectedSection()).c_str());

        //Setup ROI
        roiChanged();

        //Select a section
        if(mZs->ItemIndex == -1)
        {
            mZs->ItemIndex = 0;
        }
    }
}

void TRenderProjectFrame::fetchImagesOnProgress(void* arg1, void* arg2)
{
    int& count = * ( (int*) arg2);
	string url = mCreateCacheThread.getURL(count);
    Log(lInfo) << "Fetching: " << url;

    //Check cache for current ROI and z
    string z = stdstr(mZs->Items->Strings[count]);

    if(mExplorer.Cache.checkPresence(mRP, toInt(z), "jpeg-image"))
    {
        mZs->Checked[count] = true;
    }
}

//This is called from a thread and need to be synchronized with the UI main thread
void TRenderProjectFrame::onImage(void* arg1, void* arg2)
{
    int& count = * ( (int*) arg2);
    string z = stdstr(mZs->Items->Strings[count]);

    if(!fileExists(mRC.mFetchImageThread->getFullPathAndFileName()))
    {
        Log(lError) << "File does not exist: " << mRC.mFetchImageThread->getFullPathAndFileName();
        return;
    }

    if(mExplorer.Cache.checkPresence(mRP, toInt(z), "jpeg-image"))
    {
        mZs->Checked[count] = true;
    }

	mCurrentImageFile = mRC.mFetchImageThread->getFullPathAndFileName();
//	double val = CustomImageRotationE->getValue();
//    if(val != 0)
//    {
//		paintRotatedImage(val);
//    }
//    else
    {
        try
        {
            //Create a temporary stream
            unique_ptr<TMemoryStream> stream = unique_ptr<TMemoryStream>(new TMemoryStream());
            stream->LoadFromFile(mCurrentImageFile.c_str());
            if(stream->Size)
            {
            	stream->Position = 0;
                Image1->Picture->LoadFromStream(stream.get());
            }
        }
        catch(...)
        {
            Log(lError) << "Failed to load image file: "<<mCurrentImageFile;
            return;
        }
    }

    Image1->Refresh();
    Log(lDebug3) << "WxH = " <<Image1->Picture->Width << "x" << Image1->Picture->Height;
    this->Image1->Cursor = crDefault;
}


void TRenderProjectFrame::onROIChanged(void* arg1, void* arg2)
{
    TThread::Synchronize(NULL, &roiChanged);
}

void __fastcall TRenderProjectFrame::roiChanged()
{
    Log(lInfo) << "ROI was changed.";
    XCoordE->setValue(mCurrentROI.getX1());
    YCoordE->setValue(mCurrentROI.getY1());
    Width->setValue(mCurrentROI.getWidth());
    Height->setValue(mCurrentROI.getHeight());
    mScaleE->setValue(mCurrentROI.getScale());
    checkCache();
}

void TRenderProjectFrame::checkCache()
{
    //OtherCB
    //Todo reimplement this, to preserve any selected items, as clear remove any selected ones
    OtherCB->Clear();
    StacksCB->Clear();

    StringList stackFiles(getFilesInFolder(getCurrentROIPath(), "tif", false));
    for(int i = 0; i < stackFiles.count(); i++)
    {
        if(startsWith("stack_", stackFiles[i]))
        {
            //Setup something robust here later on
            string* item = new string(joinPath(getCurrentROIPath(), stackFiles[i]));
            stringstream itemCaption;
            itemCaption << "Stack_"<<i + 1;
            StacksCB->AddItem(vclstr(itemCaption.str()), (TObject*) item);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::StackCBChange(TObject *Sender)
{
    if(StackCB->ItemIndex == -1)
    {
		return;
    }

    string stackName(stdstr(StackCB->Text));
	mRP.setSelectedStackName(stackName);
   	populateZsForCurrentStack();

    //Populate channels
    StringList chs = mRC.StackDataAPI.getChannelsInSelectedStack(mRP);
    populateCheckListBox(chs, ChannelsCB);
	enableDisableGroupBox(imageParasGB, true);
	enableDisableGroupBox(Zs_GB, true);
	mRenderEnabled = true;
}

void TRenderProjectFrame::populateZsForCurrentStack()
{
    StringList zs = mRC.StackDataAPI.getZsForStack(mRP);
	Log(lInfo) << "Fetched "<<zs.count()<<" valid z's";
	Zs_GB->Caption = " Z Values (" + IntToStr((int) zs.count()) + ") ";

	int selectedZ(-1);

    if(mZs->SelCount)
    {
    	selectedZ = mZs->ItemIndex;
    }

    //Populate list box
	populateCheckListBox(zs, mZs);

    //Check cache.. if in cache, check the item
    for(int i = 0; i < mZs->Count; i++)
    {
        string z = dsl::stdstr(mZs->Items->Strings[i]);
		if(mExplorer.Cache.checkPresence(mRP, toInt(z), "jpeg-image"))
        {
            mZs->Checked[i] = true;
        }
    }

    if(selectedZ != -1)
    {
		mZs->ItemIndex = selectedZ;
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::ClickZ(TObject *Sender)
{
    if(mZs->ItemIndex == -1)
    {
    	return;
    }

    int z = toInt(stdstr(mZs->Items->Strings[mZs->ItemIndex]));
    mRP.setSelectedSection(z);

    //Fetch data using URL
	mRC.init("jpeg-image", z, mScaleE->getValue(), MinIntensityE->getValue(), MaxIntensityE->getValue());

    if(VisualsPC->Pages[VisualsPC->TabIndex] == TabSheet2)
    {
		this->Image1->Cursor = crHourGlass;
        StringList paras;
        paras.append(string("&maxTileSpecsToRender=150"));

    	//Image pops up in onImage callback
	    mRC.getImageInThread(z, paras, mRP.getSelectedChannelName(), mExplorer.Cache, mRP);
        mRC.assignOnImageCallback(onImage);
    }
    else
    {
//		OpenInNDVIZBtnClick(NULL);
    }

    URLE->setValue(createNDVIZURL());
    checkCache();
}


//---------------------------------------------------------------------------
void TRenderProjectFrame::paintRotatedImage(double angle)
{
    Image1->Align = alNone;
    Image1->Picture = NULL;
    TCanvas* c = Image1->Canvas;

    wstring fName(mCurrentImageFile.begin(), mCurrentImageFile.end());
    Gdiplus::Image image(fName.c_str());

    //Get native image dimensions
    Image1->Height = image.GetHeight();
	Image1->Width = image.GetWidth();

    Gdiplus::Graphics graphics(c->Handle);

    Gdiplus::PointF center(Image1->Width/2, Image1->Height/2);
    Gdiplus::Matrix matrix;

    matrix.RotateAt(angle, center);
    graphics.SetTransform(&matrix);

    c->Brush->Color = clBlack;
	c->Rectangle(0,0, PaintBox1->Width, PaintBox1->Height);

    //draw rotated image
    graphics.DrawImage(&image,0, 0, Image1->Width, Image1->Height);

    Image1->Align = alClient;
    Image1->Invalidate();
}

void __fastcall TRenderProjectFrame::ResetButtonClick(TObject *Sender)
{
	try
    {
	    //mScaleE->setValue(0.05 * ScaleConstantE->getValue());
        mCurrentROI = mRC.getLayerBoundsForZ(getCurrentZ(), mRP);
        XCoordE->setValue(mCurrentROI.getX1());
        YCoordE->setValue(mCurrentROI.getY1());
        Width->setValue(mCurrentROI.getWidth());
        Height->setValue(mCurrentROI.getHeight());
	    updateScale();
        mRP.setRegionOfInterest(mCurrentROI);

        ClickZ(NULL);
        Log(lDebug1) << "Origin: (X0,Y0) = (" << XCoordE->getValue() + Width->getValue()/2.<<"," <<YCoordE->getValue() + Height->getValue()/2.<<")";
	    checkCache();
        updateROIs();
    }
    catch (const EIdHTTPProtocolException& e)
    {
        Log(lError) << "There was an exception: " << stdstr(e.Message);
    }
    catch(...)
    {}
}

int	TRenderProjectFrame::getCurrentZ()
{
	int ii = mZs->ItemIndex;
    if(ii == -1)
    {
    	return -1;
    }

    return toInt(stdstr(mZs->Items->Strings[ii]));
}

//---------------------------------------------------------------------------
void TRenderProjectFrame::updateScale()
{
    //Scale the scaling
    double scale  = (double) Image1->Height / (double) mCurrentROI.getHeight();
    Log(lDebug5) << "Image Scale: " << scale;
	if(scale < 0.005)
    {
    	scale = 0.009;
    }
    else if(scale > 1.0)
    {
    	scale = 1.0;
    }
	mScaleE->setValue(scale);
    mCurrentROI.setScale(scale);
}

string TRenderProjectFrame::getCurrentROIPath()
{
 	return joinPath(mExplorer.Cache.getBasePath(), mRP.getProjectOwner(), mRP.getRenderProjectName(), mRP.getSelectedStackName(), mCurrentROI.getFolderName());
}

void TRenderProjectFrame::updateROIs()
{
    //Create basepath
    stringstream path;
    path << joinPath(mExplorer.Cache.getBasePath(), mRP.getProjectOwner(), mRP.getRenderProjectName(), mRP.getSelectedStackName());

    StringList rois(getSubFoldersInFolder(path.str(), false));
    populateCheckListBox(rois, ROI_CB);
	populateZsForCurrentStack();
}

//---------------------------------------------------------------------------
double TRenderProjectFrame::getImageStretchFactor()
{
	if((mScaleE->getValue() * Height->getValue() * Width->getValue()) == 0)
    {
    	Log(lError) << "Tried to divide by zero!";
    	return 1;
    }

    if(Image1->Height < Image1->Width)
    {
    	return Image1->Height / (mScaleE->getValue() * Height->getValue());
    }
    else
    {
		return Image1->Width / (mScaleE->getValue() * Width->getValue());
    }
}

//---------------------------------------------------------------------------
void TRenderProjectFrame::DrawShape(TPoint TopLeft, TPoint BottomRight, TPenMode AMode)
{
  	PaintBox1->Canvas->Pen->Mode = AMode;
	PaintBox1->Canvas->Rectangle(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::IntensityKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	if(Key != VK_RETURN)
    {
    	return;
    }
    int minInt = MinIntensityE->getValue();
    int maxInt = MaxIntensityE->getValue();
    if(mZs->ItemIndex == -1)
    {
        return;
    }

    int z = toInt(stdstr(mZs->Items->Strings[mZs->ItemIndex]));
    ClickZ(NULL);
}

void __fastcall TRenderProjectFrame::ROIKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	if(Key == VK_RETURN)
    {
        mCurrentROI = RegionOfInterest(XCoordE->getValue(), YCoordE->getValue(), Width->getValue(), Height->getValue(), mScaleE->getValue());
		ClickZ(Sender);
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::openInChromeClick(TObject *Sender)
{
    string url = URLE->getValue();
	ShellExecuteA(0,0, "chrome.exe", url.c_str(), 0, SW_SHOWMAXIMIZED);
}

string TRenderProjectFrame::createNDVIZURL()
{
    const RenderServiceParameters* rs = mRC.getRenderServiceParameters();

    string baseURL = "http://" + rs->getHost();
	string URL(baseURL + ":8001/#!{'layers':{'STACK':{'type':'image'_'source':'render://" + baseURL + "/OWNER/PROJECT/STACK'_'max':MAX_INTENSITY}}_'navigation':{'pose':{'position':{'voxelSize':[1_1_1]_'voxelCoordinates':[X_CENTER_Y_CENTER_Z_VALUE]}}_'zoomFactor':ZOOM_FACTOR}}");

	//http://localhost:8001/#!{'layers':{'TESTAcquisition_GFP':{'type':'image'_'source':'render://http://localhost/Testing/Test/TESTAcquisition_GFP'_'max':0.15259}}_'navigation':{'pose':{'position':{'voxelSize':[1_1_1]_'voxelCoordinates':[3576_5709_403]}}_'zoomFactor':1.834862}}

    double xCenter = XCoordE->getValue() + Width->getValue()/2.;
	double yCenter = YCoordE->getValue() + Height->getValue()/2.;
    URL = replaceSubstring("STACK", 	        stdstr(StackCB->Text), 	                                URL);
    URL = replaceSubstring("OWNER", 	        stdstr(OwnerE->Text), 	                                URL);
    URL = replaceSubstring("PROJECT", 	        stdstr(ProjectE->Text), 	                                URL);
    URL = replaceSubstring("MAX_INTENSITY", 	dsl::toString(2.0 * (MaxIntensityE->getValue()/65535.0)), 	URL);
    URL = replaceSubstring("X_CENTER", 			dsl::toString(xCenter), 					                URL);
    URL = replaceSubstring("Y_CENTER", 			dsl::toString(yCenter), 					                URL);
    URL = replaceSubstring("Z_VALUE", 			dsl::toString(getCurrentZ()), 	 			                URL);
    URL = replaceSubstring("ZOOM_FACTOR", 		dsl::toString(0.5*(1.0/mScaleE->getValue())), 	 			URL);
    Log(lDebug5) <<"NDVIZ url: "<< URL;
	return URL;
}

void __fastcall TRenderProjectFrame::FetchSelectedZsBtnClick(TObject *Sender)
{
    TButton* b = dynamic_cast<TButton*>(Sender);
    if(b == FetchSelectedZsBtn)
    {
        if(mCreateCacheThread.isRunning())
        {
            mCreateCacheThread.stop();
        }
        else
        {
            string imageType("jpeg-image");
		    const RenderServiceParameters* rs = mRC.getRenderServiceParameters();
            int z = toInt(stdstr(mZs->Items->Strings[0]));
            RenderClient rc;
            rc.setRenderServiceParameters(*rs);
            rc.init(imageType, z, mScaleE->getValue(), MinIntensityE->getValue(), MaxIntensityE->getValue());

            //Create image URLs
            StringList urls;
            for(int i = 0; i < mZs->Count; i++)
            {
                int	z = toInt(stdstr(mZs->Items->Strings[i]));
                urls.append(rc.getURLForZ(z, mRP));
            }

    	    StringList paras;
	        paras.append(string("&maxTileSpecsToRender=150"));
            mCreateCacheThread.setup(urls);
            mCreateCacheThread.addParameters(paras);
            mCreateCacheThread.setChannel(mRP.getSelectedChannelName());
            mCreateCacheThread.setImageType(imageType);
            mCreateCacheThread.start();
            CreateCacheTimer->Enabled = true;
        }
    }
    else if(b == ClearCacheBtn)
    {
        //Clear cache for the current owner/project/stack
        Path p(mRP.getLocalCacheFolder());

        p.append(joinPath(mRP.getProjectOwner(), mRP.getRenderProjectName()));
        p.append(mRP.getSelectedStackName());

        Log(lInfo) << "Deleting local cache for stack: " << p.toString();
        boost::filesystem::remove_all(p.toString());
        checkCache();
		updateROIs();
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::CreateTiffStackExecute(TObject *Sender)
{
    //Extract selected filenames from checked z's
    StringList sections = getCheckedItems(mZs);

    //Create commandline for imagemagicks convert program
    if(!sections.count())
    {
        Log(lError) << "No sections selected for stack creation..";
    }

	if(!mTiffStackCreator.checkForImageMagick())
    {
        MessageDlg("ImageMagick was not found. Please download from imagemagick.org.\nInstall and setup installation path on the ATExplorers settings page.", mtWarning, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    //Do something when finished..
    mTiffStackCreator.assignOpaqueData(mZs, NULL);
    mTiffStackCreator.assignCallbacks(NULL, NULL, onIMProcessFinished);

    StringList fileNames;
    for(int i = 0; i < sections.count(); i++)
    {
        int z(toInt(sections[i]));
        fileNames.append(mExplorer.Cache.getFileNameForZ(z, mRP));
    }

    //Creat output filename
    string stackFName(joinPath(getFilePath(fileNames[0]), "stack_" + mRP.getSelectedChannelName() + "_" + getUUID() + ".tif"));
    shared_ptr<TiffStackProject> stack = mTiffStackCreator.create(fileNames, stackFName);
    stack->setROI(mCurrentROI);
    stack->setSections(sections);
    stack->setIntensities(MinIntensityE->getValue(), MaxIntensityE->getValue());
    stack->appendChannel(mRP.getSelectedChannelName());
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::CreateMIPAExecute(TObject *Sender)
{
    string cvt(joinPath(mIMPath, "convert.exe"));
    Process& IMConvert = mAProcess;
    IMConvert.reset();
    IMConvert.setExecutable(cvt);
    IMConvert.setWorkingDirectory(mExplorer.Cache.getBasePath());

    //Find all stacks for current ROI
    StringList stackFiles(getFilesInFolder(mExplorer.Cache.getBasePath(), "stack_", "tif", false));

    //Create MIP's for current stack file
    string* temp = (string*) StacksCB->Items->Objects[StacksCB->ItemIndex];
    if(!temp)
    {
        Log(lError) << "Failed to extract string item";
        return;
    }

    string currentStack(*temp);
    string* mipFName = new string(getFileNameNoExtension(currentStack));

    *mipFName = "mip_" + *mipFName + ".tif";
    *mipFName = replaceSubstring("stack_", "", *mipFName);
    stringstream cmdLine;
    cmdLine << cvt <<" " << currentStack << " -monitor -evaluate-sequence max "<<*mipFName;
    Log(lInfo) << "Running convert on " << cmdLine.str();

    IMConvert.setup(cmdLine.str(), mhCatchMessages);
    IMConvert.assignCallbacks(NULL, NULL, onIMProcessFinished);

    *mipFName = joinPath(getFilePath(currentStack), *mipFName);
    IMConvert.assignOpaqueData(StacksCB, (void*) mipFName);
    IMConvert.start(true);
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::CheckBoxClick(TObject *Sender)
{
    //Open mip
    TCheckListBox* lb = dynamic_cast<TCheckListBox*>(Sender);
    if(lb == StacksCB && StacksCB->ItemIndex != -1)
    {
        string* temp = (string*) StacksCB->Items->Objects[StacksCB->ItemIndex];
        if(!temp)
        {
            Log(lError) << "Failed to extract string item";
            return;
        }


        string currentStack(getFileNameNoPathNoExtension(*temp));
		currentStack = replaceSubstring("stack_", "", currentStack);

        //Populate mips for current stack
        OtherCB->Clear();
        StringList mipFiles(getFilesInFolder(mExplorer.Cache.getBasePath(), "tif", false));
        for(int i = 0; i < mipFiles.count(); i++)
        {
            if(startsWith("mip_", mipFiles[i]))
            {
                //Setup something robust here later on
                string* item = new string(joinPath(mExplorer.Cache.getBasePath(), mipFiles[i]));
                if(item && contains(currentStack, *item))
                {
                    stringstream itemCaption;
    	            itemCaption << "MIP_" << i + 1;
	                OtherCB->AddItem(vclstr(itemCaption.str()), (TObject*) item);
                }
            }
        }
    }
    else if(lb == OtherCB)
    {
    }
}

void TRenderProjectFrame::OpenImageForm(string fName)
{
    struct TLocalArgs
    {
        string fName;
        void __fastcall sync()
        {
            TImageForm* iForm (new TImageForm("", "", NULL));
            iForm->load(fName);
            iForm->Show();
        }
    };

    TLocalArgs Args;
    Args.fName = fName;
    TThread::Synchronize(NULL, &Args.sync);
}

void TRenderProjectFrame::onIMProcessFinished(void* arg1, void* arg2)
{
    Log(lInfo) << "Process Finished";

    if(arg1 == (void*) StacksCB)
    {
        int itemIndx = StacksCB->ItemIndex;
	    checkCache();
        StacksCB->ItemIndex = itemIndx;
        StacksCB->OnClick(StacksCB);

        //Open MIP window
        if(arg2)
        {
        	string& fName = *((string*) arg2);
            if(fileExists(fName))
            {
                OpenImageForm(fName);
                delete &fName;
            }
        }
    }
    //We know this is from the creation of a tiffstack
    else if(arg1 == (void*) mZs)
    {
        int itemIndx = StacksCB->ItemIndex;
	    checkCache();
        StacksCB->ItemIndex = itemIndx;
	    shared_ptr<TiffStackProject> tiffStack = mTiffStackCreator.getStack();

        //Copy the stack to a new stack..
        //Add the stack to the project file
        TiffStackProject* stack = new TiffStackProject(*(tiffStack.get()));
        stack->setProjectName("stack_" + stack->getChannels().asString());

        //Add to the treeview and parent project XML
	    mRP.addChild(stack);
        mRP.notifyObservers(UpdateTree);
        Project* root = mRP.getProjectRoot();
        root->save();
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::ROI_CBClick(TObject *Sender)
{
    //Switch ROI
    if(ROI_CB->ItemIndex == -1)
    {
        return;
    }
    RegionOfInterest roi = RegionOfInterest(stdstr(ROI_CB->Items->Strings[ROI_CB->ItemIndex]), mCurrentROI.getScale());
    mCurrentROI = roi;
    mCurrentROI.setScale(mExplorer.Cache.getLowestResolutionInCache(mRP, mCurrentROI));
    mScaleE->setValue(mCurrentROI.getScale());
	roiChanged();
    mRP.setRegionOfInterest(mCurrentROI);
    ClickZ(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::OpenInExplorerAExecute(TObject *Sender)
{
    //Check who sender is
    TAction* a = dynamic_cast<TAction*>(Sender);
    if(!a)
    {
        return;
    }

    string fName("");

    if(a->ActionComponent == OpenSectionInExplorer)
    {
        //Get section file from z's
        if(mZs->ItemIndex != -1)
        {
            fName = mExplorer.Cache.getImageLocalCachePathAndFileName(mRP);
        }
    }
    else if(a->ActionComponent == OpenStackInExplorer)
    {
        fName = getStringFromSelectedCB(StacksCB);
	}
    else if(a->ActionComponent == OpenMIPInExplorer)
    {
        fName = getStringFromSelectedCB(OtherCB);
    }
    else if(a->ActionComponent == OpenROIInExplorer)
    {
        fName = mExplorer.Cache.getImageLocalCachePathAndFileName(mRP);
    }

    if(fName.size())
    {
        ITEMIDLIST *pidl = ILCreateFromPath(fName.c_str());
        if(pidl)
        {
            SHOpenFolderAndSelectItems(pidl,0,0,0);
            ILFree(pidl);
        }
    }
}

void TRenderProjectFrame::zoom(int zoomFactor, bool out)
{
    if(out)
    {
		zoomFactor *= (-1.0);
    }
	//Modify bounding box with x%
    mCurrentROI = RegionOfInterest(XCoordE->getValue(), YCoordE->getValue(), Width->getValue(), Height->getValue());
    mCurrentROI.zoom(zoomFactor);

	XCoordE->setValue(mCurrentROI.getX1());
    YCoordE->setValue(mCurrentROI.getY1());
    Width->setValue( mCurrentROI.getWidth());
    Height->setValue(mCurrentROI.getHeight());

    mRP.setRegionOfInterest(mCurrentROI);
    updateScale();
	roiChanged();
    updateROIs();
	ClickZ(NULL);
    checkCache();
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::RzSpinButtons1DownLeftClick(TObject *Sender)
{
	CustomImageRotationE->setValue(CustomImageRotationE->getValue() - 0.5);
	double val = CustomImageRotationE->getValue();
	paintRotatedImage(val);
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::RzSpinButtons1UpRightClick(TObject *Sender)
{
	CustomImageRotationE->setValue(CustomImageRotationE->getValue() +0.5);
	double val = CustomImageRotationE->getValue();
	paintRotatedImage(val);
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::ToggleImageGridAExecute(TObject *Sender)

{
    Log(lDebug5) << "Action Component: " << stdstr(ToggleImageGridA->ActionComponent->Name);

    TMenuItem* ac = dynamic_cast<TMenuItem*>(ToggleImageGridA->ActionComponent);
    if(ac)
    {
        ShowImageGridCB->Checked = !ShowImageGridCB->Checked;
    }
    else
    {
   		PaintBox1Paint(NULL);
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::ToggleImageGridAUpdate(TObject *Sender)
{
    if(!mIsDrawing)
    {
        PaintBox1->BringToFront();
		PaintBox1->Invalidate();
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::PaintBox1Paint(TObject *Sender)
{
    if(ShowImageGridCB->Checked)
    {
        mImageGrid.paint();
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::OtherCBDblClick(TObject *Sender)
{
    TCheckListBox* lb = dynamic_cast<TCheckListBox*>(Sender);
    if(lb != OtherCB)
    {
        return;
    }

    //Get item
    TObject* item = lb->Items->Objects[lb->ItemIndex];
    if(item)
    {
        string* fName((string*) item);
        TImageForm* iForm (new TImageForm("", "", this));
        iForm->load(*fName);
        iForm->Show();
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if(mRenderEnabled == false || Button == TMouseButton::mbRight)
    {
    	return;
    }

	double stretchFactor = getImageStretchFactor();
	if(Button == TMouseButton::mbMiddle)
    {
       	Screen->Cursor = crSize;
		mTopLeftSelCorner = Mouse->CursorPos;
		mTopLeftSelCorner = this->Image1->ScreenToClient(mTopLeftSelCorner);

		//Convert to world image coords (minus offset)
	    mTopLeftSelCorner = controlToImage(mTopLeftSelCorner, mScaleE->getValue(), stretchFactor);
        return;
    }

    mIsDrawing = true;
    PaintBox1->Canvas->MoveTo(X , Y);
    Origin = Point(X, Y);
    MovePt = Origin;

    //For selection
	mTopLeftSelCorner = Mouse->CursorPos;
	mTopLeftSelCorner = this->Image1->ScreenToClient(mTopLeftSelCorner);

	//Convert to world image coords (minus offset)
    mTopLeftSelCorner = controlToImage(mTopLeftSelCorner, mScaleE->getValue(), stretchFactor);
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
	if(mIsDrawing)
  	{
		DrawShape(Origin, MovePt, pmNotXor);
		MovePt = Point(X, Y);
		DrawShape(Origin, MovePt, pmNotXor);
  	}

	TPoint p = this->Image1->ScreenToClient(Mouse->CursorPos);
    double stretchF = getImageStretchFactor();
	XE->Caption = IntToStr((int) (p.X * stretchF) + XCoordE->getValue()) ;
	YE->Caption = IntToStr((int) (p.Y * stretchF) + YCoordE->getValue());

	//Convert to world image coords (minus offset)
    double stretchFactor = getImageStretchFactor();
    if(stretchFactor)
    {
	    p = controlToImage(p, mScaleE->getValue(), stretchFactor);
    }

	if(GetAsyncKeyState(VK_MBUTTON) < 0)
    {
    	//Move the picture
	//        Image1->Top = Image1->Top + 1;
    }
}

void __fastcall TRenderProjectFrame::MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    double stretchFactor = getImageStretchFactor();
	if(Button == TMouseButton::mbMiddle)
    {
	  	Screen->Cursor = crDefault;
    	TPoint p2;
		p2 = Mouse->CursorPos;
		p2 = this->Image1->ScreenToClient(p2);
	    p2 = controlToImage(p2, mScaleE->getValue(), stretchFactor);

		//Convert to world image coords (minus offset)
		XCoordE->setValue(XCoordE->getValue() + (mTopLeftSelCorner.X - p2.X));
		YCoordE->setValue(YCoordE->getValue() + (mTopLeftSelCorner.Y - p2.Y));

		mCurrentROI = RegionOfInterest(XCoordE->getValue(), YCoordE->getValue(), Width->getValue(), Height->getValue(), mScaleE->getValue());
       	ClickZ(Sender);
    }

	if(!mIsDrawing || (Button == TMouseButton::mbRight))
    {
    	return;
    }

	mIsDrawing = false;

    //For selection
	mBottomRightSelCorner = this->Image1->ScreenToClient(Mouse->CursorPos);

	//Convert to world image coords (minus offset)
    mBottomRightSelCorner = controlToImage(mBottomRightSelCorner, mScaleE->getValue(), stretchFactor);

	//Check if selection indicate a 'reset'
	if(mBottomRightSelCorner.X - mTopLeftSelCorner.X <= 0 || mBottomRightSelCorner.Y - mTopLeftSelCorner.Y <= 0)
    {
    	ResetButtonClick(NULL);
		return;
    }

	mCurrentROI = RegionOfInterest(	XCoordE->getValue() + mTopLeftSelCorner.X,
    				 				YCoordE->getValue() + mTopLeftSelCorner.Y,
                                    mBottomRightSelCorner.X - mTopLeftSelCorner.X,
                                    mBottomRightSelCorner.Y - mTopLeftSelCorner.Y,
                                    mScaleE->getValue());

    mRP.setRegionOfInterest(mCurrentROI);
    updateScale();
	roiChanged();
    updateROIs();
	ClickZ(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::CreateCacheTimerTimer(TObject *Sender)
{
	if(mCreateCacheThread.isRunning())
    {
		FetchSelectedZsBtn->Caption = "Stop";
    }
    else
    {
		CreateCacheTimer->Enabled = false;
		FetchSelectedZsBtn->Caption = "Generate";
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::Button1Click(TObject *Sender)
{
	updateROIs();
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::openVolumesForm(TObject *Sender)
{
    //Open creat volumes form
	if(!mCreateVolumesForm)
    {
		//	TCreateLocalVolumesForm(RenderProject& rp, RenderLocalCache& cache, const string& imageMagickPath, TComponent* Owner);
    	mCreateVolumesForm = new TCreateLocalVolumesForm(mRP, mExplorer.Cache, mIMPath, this);
    }

    StringList stacks;
    stacks.append(stdstr(StackCB->Text));
    mCreateVolumesForm->populate(mCurrentROI, stacks);
    mCreateVolumesForm->Show();
}

TPoint controlToImage(const TPoint& p, double scale, double stretchFactor)
{
	TPoint pt;
    if(scale > 0)
    {
    	pt.X = (p.X / scale) / stretchFactor;
	    pt.Y = (p.Y / scale) / stretchFactor;
    }
	return pt;
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::ChannelsCBClick(TObject *Sender)
{
	int indx(-1);
    for(int i = 0; i < ChannelsCB->Count; i++)
    {
    	if(ChannelsCB->Selected[i])
        {
            indx = i;
            break;
        }
    }

    ChannelsCB->CheckAll(cbUnchecked);
    ChannelsCB->Checked[indx] = cbChecked;

    string channel = stdstr(ChannelsCB->Items->Strings[indx]);

    //Render this channel
    mRP.setSelectedChannelName(channel);

	populateZsForCurrentStack();
    ClickZ(NULL);
}

void __fastcall TRenderProjectFrame::ChannelsCBClickCheck(TObject *Sender)
{
	ChannelsCBClick(Sender);
}

void __fastcall TRenderProjectFrame::HeaderControl1SectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section)
{
    TPoint p = HeaderControl->ClientToScreen(TPoint(0,0));

    if(Section->Text == '+')
    {
	    zoom(mZoomFactor->getValue(), false);
    }
    else if(Section->Text == '-')
    {
	    zoom(mZoomFactor->getValue(), true);
    }
    else if(Section->Text == "Misc")
    {
	    int nrOfItems  = MiscPopup->Items->Count;
    	MiscPopup->Popup(p.x + Section->Left, p.y - (HeaderControl->Height + nrOfItems * 16));
    }
    else if(Section->Text == "Image Grid")
    {
		ShowImageGridCB->Checked = !ShowImageGridCB->Checked;
    }
}

void __fastcall TRenderProjectFrame::Panel1ContextPopup(TObject *Sender, TPoint &MousePos,
          bool &Handled)
{
    TPoint popupCoord = Panel1->ClientToScreen(MousePos);
    ImagePopup->Popup(popupCoord.X, popupCoord.Y);
    Handled = true;
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::TabSheet2ContextPopup(TObject *Sender, TPoint &MousePos,
          bool &Handled)
{
//    ImagePopup->Popup(MousePos.X, MousePos.Y);
//    Handled = true;
}

void __fastcall TRenderProjectFrame::StacksCBClick(TObject *Sender)
{
    //
}

void __fastcall TRenderProjectFrame::CheckZs(TObject *Sender)
{
	TMenuItem* item = dynamic_cast<TMenuItem*>(Sender);
    if(item == CheckAll)
    {
		mZs->CheckAll(cbChecked);
    }
    else if(item == UnCheckAll)
    {
		mZs->CheckAll(cbUnchecked);
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::CreateSubVolumeStackAExecute(TObject *Sender)
{
    //Create a subvolume stack in render
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::HeaderControl1ContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled)
{
;
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::RefreshStacksBtnClick(TObject *Sender)
{
	populate();
}

//---------------------------------------------------------------------------
void __fastcall TRenderProjectFrame::CustomImageRotationEKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift)
{
    if(Key == VK_RETURN)
    {
    	double val = CustomImageRotationE->getValue();
		paintRotatedImage(val);
    }
}


