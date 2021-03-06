#ifndef TSelectPointMatchCollectionProjectFormH
#define TSelectPointMatchCollectionProjectFormH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "dslTIntegerLabeledEdit.h"
#include "dslTSTDStringLabeledEdit.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "atRenderClient.h"
#include "atRenderServiceParameters.h"
#include "atATExplorer.h"
#include "pointMatches/atPointMatchCollection.h"

//---------------------------------------------------------------------------
using at::RenderClient;
using at::RenderServiceParameters;
using at::RenderProject;
using at::ATExplorer;
using at::PointMatchCollection;
//---------------------------------------------------------------------------
class PACKAGE TSelectPointmatchCollectionProjectForm : public TForm
{
    __published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
	TComboBox *CollectionCB;
        TComboBox *OwnerCB;
        TLabel *Label1;
        TLabel *Label2;
        TPanel *Panel1;
        TButton *Button1;
        TButton *Button2;
		TButton *PopulateOwnersBtn;
		TComboBox *RenderServicesCB;
	TPanel *Panel2;
		void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
		void __fastcall OwnerCBChange(TObject *Sender);
		void __fastcall PopulateOwnersBtnClick(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
        void __fastcall RenderServicesCBCloseUp(TObject *Sender);
        void __fastcall RenderServicesCBChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

    private:
        RenderProject                   mRP;
        RenderClient                    mRC;
        ATExplorer&                     mExplorer;

    public:
        					__fastcall 	TSelectPointmatchCollectionProjectForm(ATExplorer& e, TComponent* Owner);
        string                          getRenderOwner();
        PointMatchCollection*              getPointMatchCollection();
        RenderServiceParameters        getRenderService();
};

extern PACKAGE TSelectPointmatchCollectionProjectForm *SelectPointmatchCollectionProjectForm;

#endif
