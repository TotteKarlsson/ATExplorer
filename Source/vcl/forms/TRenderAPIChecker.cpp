#pragma hdrstop
#include "TRenderAPIChecker.h"
#include "atRenderProject.h"
#include "atRenderClient.h"
#include "dslVCLUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "dslTSTDStringLabeledEdit"
#pragma resource "*.dfm"
TRenderAPIChecker *RenderAPIChecker;


using namespace at;
using namespace dsl;
//---------------------------------------------------------------------------
__fastcall TRenderAPIChecker::TRenderAPIChecker(RenderServiceParameters& rsp, TComponent* Owner)
	: TForm(Owner),
    mHost(rsp)
{}

//---------------------------------------------------------------------------
void __fastcall TRenderAPIChecker::RequestBtnClick(TObject *Sender)
{
    //Get some render owners
    RenderProject dummyProject("Dummy", mHost);
    RenderClient   renderClient ;
    renderClient.setRenderServiceParameters(mHost);
	ResponseMemo->Clear();
	StringList response;


    if(APIs->ItemIndex == 0)
    {
        //Get server properties
	    string r = renderClient.ServerConfigurationAPI.getServerProperties();
        response = StringList(r, '\n');
    }
    else if(APIs->ItemIndex == 1)
    {
	    response = renderClient.StackDataAPI.getOwners();
    }

    RequestURL->setValue(renderClient.getLastRequestURL());

    //Populate memo with response
    for(int i = 0; i < response.count(); i++)
    {
		ResponseMemo->Lines->Add(vclstr(response[i]));
    }
}

//---------------------------------------------------------------------------
void __fastcall TRenderAPIChecker::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    if(Key == VK_ESCAPE)
    {
        Close();
    }
}

