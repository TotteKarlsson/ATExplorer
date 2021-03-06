#pragma hdrstop
#include "atRenderProjectItemView.h"
#include "atRenderProject.h"
#include "dslLogger.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

namespace at
{

using namespace dsl;
RenderProjectItemView::RenderProjectItemView(TPageControl& pc, ATExplorer& e, RenderProject& rp)
:
ProjectItemTabbedView(pc, rp)
{
    mTabSheet->Caption = rp.getProjectName().c_str();
    mRenderProjectFrame = unique_ptr<TRenderProjectFrame>(new TRenderProjectFrame(e, rp, &mPC));
	mRenderProjectFrame->populate();
    mRenderProjectFrame->Parent =  mTabSheet.get();
    mRenderProjectFrame->Align = alClient;
}

RenderProjectItemView::~RenderProjectItemView()
{
    Log(lDebug3) << "Closing RenderProjectItemView..";
}

}

