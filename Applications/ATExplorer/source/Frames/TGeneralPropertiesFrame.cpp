#include <vcl.h>
#pragma hdrstop
#include "TGeneralPropertiesFrame.h"
#include "dslVCLUtils.h"
#include "dslFileUtils.h"
#include "dslLogger.h"
#include "atDockerContainer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "dslTIntegerLabeledEdit"
#pragma link "dslTPropertyCheckBox"
#pragma link "dslTSTDStringLabeledEdit"
#pragma resource "*.dfm"
TGeneralPropertiesFrame *GeneralPropertiesFrame;
//---------------------------------------------------------------------------


using namespace dsl;
using namespace at;
//---------------------------------------------------------------------------
__fastcall TGeneralPropertiesFrame::TGeneralPropertiesFrame(ATExplorer& e, TComponent* Owner)
	:
    TFrame(Owner),
    mExplorer(e)
    {}

//---------------------------------------------------------------------------
bool TGeneralPropertiesFrame::populate(Properties& props)
{
    props.disableEdits();
	ImageMagickPathE				->assignExternalProperty(dynamic_cast< Property<string>* >(props.getProperty("IMAGE_MAGICK_PATH")), 			false);
    props.enableEdits();


    //Fill out render and docker backends
    RenderPythonContainersCB->Clear();
    DockerContainer* c =  mExplorer.getFirstDockerContainer();
    while(c)
    {
		RenderPythonContainersCB->AddItem(c->getName().c_str(), (TObject*) c);
        c = mExplorer.getNextDockerContainer();
    }

    RenderServicesCB->Clear();
    RenderServiceParameters* rs =  mExplorer.getFirstRenderService();
    while(rs)
    {
		RenderServicesCB->AddItem(rs->getName().c_str(), (TObject*) rs);
        rs = mExplorer.getNextRenderService();
    }

	BaseProperty* p = props.getProperty("DEFAULT_RENDER_PYTHON_CONTAINER");
    if(p)
    {
        if(p->getValueAsString().size())
        {
            //Select
        }
    }
    return true;
}

//---------------------------------------------------------------------------
void __fastcall TGeneralPropertiesFrame::BrowseForFolderClick(TObject *Sender)
{
    TButton* btn = dynamic_cast<TButton*>(Sender);

    if(btn == BrowseForImageMagickPathBtn)
    {
        //Browse for folder
        string res = browseForFolder(ImageMagickPathE->getValue());
        if(folderExists(res))
        {
            ImageMagickPathE->setValue(res);
        }
        else
        {
            Log(lWarning) << "Image Magick Path was not set..";
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TGeneralPropertiesFrame::TestRenderServiceBtnClick(TObject *Sender)
{
    //Get some render owners
    MessageDlg("Not Implemented yet", mtInformation, TMsgDlgButtons() << mbOK, 0);
}

//---------------------------------------------------------------------------
void __fastcall TGeneralPropertiesFrame::RenderPythonContainersCBChange(TObject *Sender)
{
	//Select default RenderPythonContainer

    int ii = RenderPythonContainersCB->ItemIndex;
	if(ii == -1)
    {
        return;
    }

    //Get item
    string item = stdstr(RenderPythonContainersCB->Items->Strings[ii]);
	Property<string>* p = dynamic_cast<Property<string>*>(mExplorer.Properties.getProperty("DEFAULT_RENDER_PYTHON_CONTAINER"));
    if(p)
    {
        p->setValue(item);
    }
}


