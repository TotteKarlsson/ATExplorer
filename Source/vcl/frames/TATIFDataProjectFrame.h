#ifndef TATIFDataProjectFrameH
#define TATIFDataProjectFrameH
#include "atATIFDataProject.h"
#include <System.Classes.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include "dslTSTDStringLabeledEdit.h"
#include "atPopulateATDataThread.h"
#include <Vcl.FileCtrl.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Outline.hpp>
#include "atATExplorer.h"
#include "dslTIntegerLabeledEdit.h"
//---------------------------------------------------------------------------
using at::ATIFDataProject;
using at::ATExplorer;
//---------------------------------------------------------------------------
class PACKAGE TATIFDataProjectFrame : public TFrame
{
    __published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TSTDStringLabeledEdit *DataRootFolderE;
        TButton *ScanDataBtn;
        TGroupBox *GroupBox2;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *NrOfRibbonsLbl;
        TLabel *NrOfSectionsLbl;
        TLabel *NrOfSessionsLbl;
        TLabel *NrOfTilesLbl;
        TProgressBar *PopulatePB;
        TLabel *Label5;
        TLabel *NrOfChannelsLbl;
	TPanel *Panel1;
	TButton *CreateStateTablesBtn;
	TLabel *Label6;
	TLabel *StateTablesLbl;
	TButton *CreateRenderStacksBtn;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TPanel *Panel3;
	TButton *CreateMediansBtn;
	TButton *ApplyMediansBtn;
	TGroupBox *GroupBox3;
	TLabel *Label8;
	TComboBox *RenderServicesCB;
	TGroupBox *GroupBox4;
	TGroupBox *GroupBox5;
	TIntegerLabeledEdit *StartSectionE;
	TIntegerLabeledEdit *EndSectionE;
	TIntegerLabeledEdit *IntegerLabeledEdit1;
	TIntegerLabeledEdit *IntegerLabeledEdit2;
        void __fastcall ScanDataBtnClick(TObject *Sender);
	void __fastcall CreateStateTablesBtnClick(TObject *Sender);
//	void __fastcall RenderPythonContainersCBChange(TObject *Sender);

    private:
        ATExplorer&		  		    	    mExplorer;

                                            //A Reference to a atifdata project
        ATIFDataProject&		      	    mProject;
        void                                populate();
        at::PopulateATDataThread            mPopulateDataThread;

        void								onThreadEnter(void*, 	void*, void*);
        void								onThreadProgress(void*, void*, void*);
        void								onThreadExit(void*, 	void*, void*);

    public:
    						__fastcall 		TATIFDataProjectFrame(ATExplorer& e, ATIFDataProject& rp, TComponent* Owner);
};

extern PACKAGE TATIFDataProjectFrame *ATIFDataProjectFrame;
#endif
