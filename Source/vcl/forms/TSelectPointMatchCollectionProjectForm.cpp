#pragma hdrstop
#include "TSelectPointMatchCollectionProjectForm.h"
#include "dslStringList.h"
#include "dslVCLUtils.h"
#include "dslLogger.h"
#include "dslFileUtils.h"
#include "atGenericListOfPointers.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "dslTIntegerLabeledEdit"
#pragma link "dslTSTDStringLabeledEdit"
#pragma resource "*.dfm"
TSelectPointmatchCollectionProjectForm *SelectPointmatchCollectionProjectForm;
//---------------------------------------------------------------------------

using namespace dsl;
using namespace at;
//---------------------------------------------------------------------------
__fastcall TSelectPointmatchCollectionProjectForm::TSelectPointmatchCollectionProjectForm(ATExplorer& e, TComponent* Owner)
	: TForm(Owner),
    mRP("", "", "" , ""),
    mRC(),
    mExplorer(e)
{
    try
    {
        RenderServicesCB->Clear();
        RenderServiceParameters* rs =  mExplorer.getFirstRenderService();
        while(rs)
        {
            RenderServicesCB->AddItem(rs->getName().c_str(), (TObject*) rs);
            rs = mExplorer.getNextRenderService();
        }
    }
    catch(...)
    {
//        Log(lError) << "We were not able to connect to host: " << BaseURLE->getValue();
    }
}

string TSelectPointmatchCollectionProjectForm::getRenderOwner()
{
    return stdstr(OwnerCB->Text);
}

PointMatchCollection* TSelectPointmatchCollectionProjectForm::getPointMatchCollection()
{
    return (PointMatchCollection*) CollectionCB->Items->Objects[CollectionCB->ItemIndex];
}

RenderServiceParameters TSelectPointmatchCollectionProjectForm::getRenderService()
{
	int index = RenderServicesCB->ItemIndex;
    if(index == -1)
    {
        return RenderServiceParameters();
    }
	RenderServiceParameters* service = (RenderServiceParameters*) RenderServicesCB->Items->Objects[index];
    return *service;
}
//---------------------------------------------------------------------------
void __fastcall TSelectPointmatchCollectionProjectForm::FormCloseQuery(TObject *Sender,
          bool &CanClose)
{
    if(this->ModalResult == mrOk)
    {
        if(OwnerCB->Text.Length() < 1 || CollectionCB->Text.Length() < 1)
        {
            MessageDlg("Please select an Owner and a Project", mtInformation, TMsgDlgButtons() << mbOK, 0);
            CanClose = false;
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TSelectPointmatchCollectionProjectForm::OwnerCBChange(TObject *Sender)
{
    //Populate projects
    const PointMatchCollections& pmcs = mRC.PointMatchAPI.getPointMatchCollectionsForOwner(stdstr(OwnerCB->Text));
    if(pmcs.count())
    {
	    PointMatchCollectionSP pmc = pmcs.getFirst();
		CollectionCB->Clear();
        while(pmc)
        {

            CollectionCB->AddItem(pmc->getName().c_str(), (TObject*) pmc.get());
            pmc = pmcs.getNext();
        }

        CollectionCB->ItemIndex = 0;
        CollectionCB->Text = CollectionCB->Items->Strings[0];
    }
}

//---------------------------------------------------------------------------
void __fastcall TSelectPointmatchCollectionProjectForm::PopulateOwnersBtnClick(TObject *Sender)
{
    //Populate owners
    StringList o = mRC.StackDataAPI.getOwners();
    if(o.size())
    {
		populateDropDown(o, OwnerCB);
        OwnerCB->ItemIndex = 0;
        OwnerCB->Text = OwnerCB->Items->Strings[0];
		OwnerCBChange(NULL);
    }
}

//---------------------------------------------------------------------------
void __fastcall TSelectPointmatchCollectionProjectForm::FormKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift)
{
    if(Key == VK_ESCAPE)
	{
 	    Close();
     }
}

//---------------------------------------------------------------------------
void __fastcall TSelectPointmatchCollectionProjectForm::RenderServicesCBCloseUp(TObject *Sender)
{
    mRC.setRenderServiceParameters(getRenderService());
	PopulateOwnersBtnClick(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TSelectPointmatchCollectionProjectForm::RenderServicesCBChange(TObject *Sender)

{
    mRC.setRenderServiceParameters(getRenderService());
}

//---------------------------------------------------------------------------
void __fastcall TSelectPointmatchCollectionProjectForm::FormShow(TObject *Sender)
{
    //Select default render server
    if(!RenderServicesCB->Items->Count)
    {
        MessageDlg("There are no renderservers available. Setup one on the options page.", mtInformation, TMsgDlgButtons() << mbOK, 0);
	    enableDisablePanel(this->Panel2, false);
		Button2->Enabled = false;
    }

	RenderServicesCB->ItemIndex = 0;
	PopulateOwnersBtnClick(NULL);
}


