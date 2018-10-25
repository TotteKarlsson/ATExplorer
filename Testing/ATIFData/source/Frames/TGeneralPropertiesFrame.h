#ifndef TGeneralPropertiesFrameH
#define TGeneralPropertiesFrameH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "dslTIntegerLabeledEdit.h"
#include "dslTPropertyCheckBox.h"
#include "dslTSTDStringLabeledEdit.h"
#include <Vcl.ExtCtrls.hpp>
#include "dslProperties.h"

using dsl::Properties;
//---------------------------------------------------------------------------
class PACKAGE TGeneralPropertiesFrame : public TFrame
{
    __published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox4;
        TSTDStringLabeledEdit *BaseURLE;
        TButton *TestRenderServiceBtn;
        TIntegerLabeledEdit *maxTileSpecsToRenderE;
        TIntegerLabeledEdit *RenderPort;
        TPropertyCheckBox *ConnectSSHServersOnStartupCB;
	TGroupBox *GroupBox2;
	TSTDStringLabeledEdit *ImageMagickPathE;
	TButton *BrowseForImageMagickPathBtn;
		void __fastcall BrowseForFolderClick(TObject *Sender);
	void __fastcall TestRenderServiceBtnClick(TObject *Sender);

    private:

    public:
        			__fastcall 	TGeneralPropertiesFrame(TComponent* Owner);
        bool                	populate(Properties& props);
};

extern PACKAGE TGeneralPropertiesFrame *GeneralPropertiesFrame;
#endif