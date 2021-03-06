#ifndef TMainFormH
#define TMainFormH
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdActns.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include "dslTLogMemoFrame.h"
#include "dslTRegistryForm.h"
#include "atProjectsTreeView.h"
#include "dslObserver.h"
#include <memory>
#include "atRenderProjectItemView.h"
#include "dslSharedPointer.h"
#include "atTreeItemObservers.h"
//---------------------------------------------------------------------------

using at::ProjectsTreeView;
using at::ATExplorerProject;
using at::RenderProjectItemView;
using at::TreeItemObservers;
using dsl::Project;
using dsl::Observer;
using std::vector;

class PACKAGE TMainForm : public TRegistryForm
{
__published:	// IDE-managed Components
	TTimer *ShutDownTimer;
	TPanel *BottomPanel;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *Help1;
	TMenuItem *About1;
	TMenuItem *Exit1;
	TMenuItem *Options1;
	TMenuItem *ThemesMenu;
	TStatusBar *StatusBar1;
	TMenuItem *Open1;
	TActionList *MenuActions;
	TFileOpen *FileOpen1;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TMenuItem *New1;
	TAction *NewProjectA;
	TSaveDialog *SaveDialog1;
	TAction *SaveProjectA;
	TMenuItem *Save1;
	TAction *SaveProjectAsA;
	TMenuItem *SaveAs1;
	TAction *CloseProjectA;
	TMenuItem *Close1;
	TImageList *ImageList1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TPanel *ProjectManagerPanel;
	TMenuItem *N1;
	TMenuItem *ReopenMenu;
	TMenuItem *N3;
	TTreeView *ProjectTView;
	TPopupMenu *ATIFDataPopup;
	TAction *AddRenderProject;
	TPanel *MainPanel;
	TAction *EditViewNode;
	TPanel *TopPanel2;
	TPanel *Panel3;
	TButton *ShowBottomPanelBtn;
	TSplitter *Splitter2;
	TPopupMenu *PopupMenu1;
	TAction *ToggleBottomPanelA;
	TMenuItem *Action11;
	TToolButton *ToolButton4;
	TAction *ToggleImageGridA;
	TAction *CreateTiffStackA;
	TAction *CreateMIPA;
	TAction *OpenSettingsA;
	TMenuItem *Settings1;
	TToolButton *ToolButton6;
	TSplitter *Splitter1;
	TFileExit *FileExit1;
	TToolButton *ToolButton7;
	TMenuItem *ProjectOptions1;
	TPageControl *MainPC;
	TPopupMenu *MainPCPopup;
	TMenuItem *Close3;
	TMenuItem *Properties1;
	TAction *RemoveFromProjectA;
	TMenuItem *RemoveFromProject1;
	TMenuItem *Rename;
	TAction *OpenProjectOptionsA;
	TAction *AddATIFDataAction;
	TPopupMenu *RenderProjectPopup;
	TMenuItem *MenuItem1;
	TMenuItem *MenuItem4;
	TMenuItem *MenuItem5;
	TPopupMenu *ExplorerProjectPopup;
	TMenuItem *MenuItem2;
	TMenuItem *MenuItem7;
	TMenuItem *AddRenderProject1;
	TMenuItem *AddATIFData1;
	TAction *OpenViewA;
	TMenuItem *CloseProject1;
	TMenuItem *Open3;
	TToolButton *ToolButton5;
	TAction *OpenAboutA;
	TAction *AddPointMatchCollectionA;
	TMenuItem *AddaPointmatchcontext1;
	TPopupMenu *PointMatchCollectionPopup;
	TMenuItem *MenuItem3;
	TMenuItem *MenuItem6;
	TMenuItem *MenuItem8;
	TLogMemoFrame *LogMemoFrame1;
	TPopupMenu *TiffStackPopup;
	TMenuItem *Delete;
	TMenuItem *OpenROI1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ShutDownTimerTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall About1Click(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ThemesMenuClick(TObject *Sender);
	void __fastcall NewProjectAExecute(TObject *Sender);
	void __fastcall FileOpen1Accept(TObject *Sender);
	void __fastcall CloseProjectAExecute(TObject *Sender);
	void __fastcall CloseProjectAUpdate(TObject *Sender);
	void __fastcall SaveProjectAsAUpdate(TObject *Sender);
	void __fastcall SaveProjectAUpdate(TObject *Sender);
	void __fastcall SaveProjectAExecute(TObject *Sender);
	void __fastcall SaveProjectAsAExecute(TObject *Sender);
	void __fastcall AddRenderProjectExecute(TObject *Sender);
	void __fastcall ProjectTViewContextPopup(TObject *Sender, TPoint &MousePos,
          bool &Handled);
	void __fastcall ProjectTViewEditing(TObject *Sender, TTreeNode *Node, bool &AllowEdit);
	void __fastcall ProjectTViewEdited(TObject *Sender, TTreeNode *Node, UnicodeString &S);
	void __fastcall EditViewNodeExecute(TObject *Sender);
	void __fastcall ProjectTViewClick(TObject *Sender);
	void __fastcall ToggleBottomPanelAExecute(TObject *Sender);
	void __fastcall ToggleBottomPanelAUpdate(TObject *Sender);
	void __fastcall OpenSettingsAExecute(TObject *Sender);
	void __fastcall ATIFDataPopupPopup(TObject *Sender);
	void __fastcall ProjectTViewDblClick(TObject *Sender);
	void __fastcall MainPCContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall Close3Click(TObject *Sender);
	void __fastcall RemoveFromProjectAExecute(TObject *Sender);
	void __fastcall RenameClick(TObject *Sender);
	void __fastcall OpenProjectOptionsAExecute(TObject *Sender);
	void __fastcall AddATIFDataActionExecute(TObject *Sender);
	void __fastcall OpenViewAExecute(TObject *Sender);
	void __fastcall OpenAboutAExecute(TObject *Sender);
	void __fastcall AddPointMatchCollectionAExecute(TObject *Sender);
	void __fastcall OpenROI1Click(TObject *Sender);

	private:
		bool          									mIsStyleMenuPopulated;

                                                        //!Practical Container for 'ItemViews'
        TreeItemObservers                               mTreeItemObservers;

                                                        //!Object that manages data and
                                                        //!behaviour in the TreeView
	    ProjectsTreeView                                mPTreeView;

		int 		 									saveProject(Project* p);
		int			 									saveProjectAs(Project* p);
		void										    selectTabForTreeItem(Project* p);
	public:
		__fastcall 			  							TMainForm(TComponent* Owner);
		__fastcall 			  							~TMainForm();


	BEGIN_MESSAGE_MAP
//		MESSAGE_HANDLER(FINISHED_RENDER_ROTATE,	    	TextMessage,		onFinishedRenderRotate);
	END_MESSAGE_MAP(TForm)
};

extern PACKAGE TMainForm *MainForm;
#endif
