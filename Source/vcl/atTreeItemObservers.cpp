#pragma hdrstop
#include "atTreeItemObservers.h"
#include "dslProject.h"
#include "dslLogger.h"
#include "atATExplorerProject.h"
#include "atRenderProject.h"
#include "atPointMatchCollectionProject.h"
#include "atATIFDataProject.h"
#include "atSession.h"
#include "atSection.h"
#include "atRibbon.h"
#include "atATIFDataProjectItemView.h"
#include "atRenderProjectItemView.h"
#include "atPointMatchCollectionProjectItemView.h"
#include "atTextFile.h"
#include "atTiffStackProject.h"
#include "atTiffStackProjectItemView.h"
#include "atTextFileItemView.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

namespace at
{

using namespace dsl;
using namespace std;

TreeItemObservers::TreeItemObservers(TPageControl& pc, ATExplorer& e)
:
MainPC(pc),
mExplorer(e)
{
    this->mObserverTag = "TreeItemObservers";
}

TreeItemObservers::~TreeItemObservers()
{
	this->setInActive();
}

bool TreeItemObservers::selectTabWithView(ProjectItemTabbedView* v)
{
    if(!v)
    {
        return false;
    }

    TTabSheet* theSheet = getTabForSubject(v->getSubject());

    for(int i = 0; i < MainPC.PageCount; i++)
    {
        TTabSheet* sh = MainPC.Pages[i];
        if(sh == theSheet)
        {
			MainPC.TabIndex = i;
            return true;
        }
    }
    return false;
}

ProjectItemTabbedView* TreeItemObservers::createView(Subject* eo)
{
	if(!eo)
    {
        return false;
    }
    Log(lInfo) << "ItemType: " << eo->getTypeName();

    TTabSheet* sh = getTabForSubject(eo);
    if(sh)
    {
        MainPC.ActivePage = sh;
        return dynamic_cast<ProjectItemTabbedView*>(eo);
    }

    else if(dynamic_cast<ATIFDataProject*>(eo))
    {
        ATIFDataProject* o = dynamic_cast<ATIFDataProject*>(eo);
        if(o)
        {
            Log(lInfo) << "Creating a ATIFData project view";
	       	shared_ptr<ATIFDataProjectItemView> aView(new ATIFDataProjectItemView(mExplorer, MainPC, *o));
        	mViews.push_back(aView);
		    this->observe(aView->getSubject());
            return aView.get();
        }
    }
    else if(dynamic_cast<RenderProject*>(eo))
    {
        RenderProject* o = dynamic_cast<RenderProject*>(eo);
        if(o)
        {
            Log(lInfo) << "Creating a Render ProjectView";
            shared_ptr<RenderProjectItemView> aItemView(new RenderProjectItemView(MainPC,  mExplorer, *o));
           	mViews.push_back(aItemView);
            this->observe(aItemView->getSubject());
            return aItemView.get();
        }
    }

    else if(dynamic_cast<PointMatchCollectionProject*>(eo))
    {
        PointMatchCollectionProject* o = dynamic_cast<PointMatchCollectionProject*>(eo);
        if(o)
        {
            Log(lInfo) << "Creating a PointMatchCollection ProjectView";
            shared_ptr<PointMatchCollectionProjectItemView> aItemView(new PointMatchCollectionProjectItemView(MainPC,  mExplorer, *o));
           	mViews.push_back(aItemView);
            this->observe(aItemView->getSubject());
            return aItemView.get();
        }
    }
    else if(dynamic_cast<TiffStackProject*>(eo))
    {
        TiffStackProject* o = dynamic_cast<TiffStackProject*>(eo);
        if(o)
        {
            Log(lInfo) << "Creating a stack view";
            shared_ptr<TiffStackProjectItemView> aItemView(new TiffStackProjectItemView(MainPC,  mExplorer, *o));
           	mViews.push_back(aItemView);
            this->observe(aItemView->getSubject());
            return aItemView.get();
        }
    }
    else if(dynamic_cast<TextFile*>(eo))
    {
        TextFile* o = dynamic_cast<TextFile*>(eo);
        if(o)
        {
            Log(lInfo) << "Creating a TextFile view";
            shared_ptr<TextFileItemView> aItemView(new TextFileItemView(MainPC,  *o));
           	mViews.push_back(aItemView);
            this->observe(aItemView->getSubject());
            return aItemView.get();
        }
    }
    else if(dynamic_cast<ATExplorerProject*>(eo))
    {
        ATExplorerProject* o = dynamic_cast<ATExplorerProject*>(eo);
        if(o)
        {
            Log(lInfo) << "Clicked on ATExplorerProject project item: " << o->getProjectName();
        }
    }

    else if(dynamic_cast<Channel*>(eo))
    {
        Channel* o = dynamic_cast<Channel*>(eo);
        if(o)
        {
            Log(lInfo) << "Clicked on channel item: " << o->getLabel();
        }
    }
    else if(dynamic_cast<Session*>(eo))
    {
        Session* o = dynamic_cast<Session*>(eo);
        if(o)
        {
            Log(lInfo) << "Clicked on session item: " << o->getLabel();
        }
    }
    else if(dynamic_cast<Ribbon*>(eo))
    {
        Ribbon* o = dynamic_cast<Ribbon*>(eo);
        if(o)
        {
            Log(lInfo) << "Clicked on Ribbon item: " << o->getAlias();
        }
    }

    return nullptr;
}

void TreeItemObservers::closeAll()
{
	mViews.clear();
}

bool TreeItemObservers::removeView(ProjectItemTabbedView* v)
{
    //Find the view
    for(int i = 0; i < mViews.size(); i++)
    {
        shared_ptr<ProjectItemTabbedView> aView = mViews[i];

        if(aView && aView.get()  == v )
        {
            mViews.erase(mViews.begin() + i);
            return true;
        }
    }
    return false;
}

unsigned int TreeItemObservers::count()
{
    return mViews.size();
}

//Just deleting the view.. not the subject..
bool TreeItemObservers::removeViewOnTabSheet(TTabSheet* ts)
{
    //Find observer object
    for(int i = 0; i < mViews.size(); i++)
    {
        shared_ptr<ProjectItemTabbedView> rpv = mViews[i];
        if(rpv && rpv->getTabSheet() == ts)
        {
            //Tell this observer to go away...
            return removeViewForSubject(rpv->getSubject());
        }
    }
    return false;
}

//Should not be needed..
bool TreeItemObservers::removeViewForSubject(Subject* p)
{
    //Find observer object
    for(int i = 0; i < mViews.size(); i++)
    {
        shared_ptr<ProjectItemTabbedView> rpv = mViews[i];
        p->detachObserver(rpv.get());

        if(rpv && rpv->getSubject() == p )
        {
            mViews.erase(mViews.begin() + i);
            return true;
        }
    }
    return false;
}

ProjectItemTabbedView* TreeItemObservers::getFirst()
{
    if(!mViews.size())
    {
        return nullptr;
    }

	mViewsIterator = mViews.begin();
    return (*mViewsIterator).get();
}


ProjectItemTabbedView* TreeItemObservers::getNext()
{
    if(mViewsIterator != mViews.end())
    {
        mViewsIterator++;
        if(mViewsIterator != mViews.end())
        {
            return (*mViewsIterator).get();
        }
    }
    return nullptr;
}

TTabSheet* TreeItemObservers::getTabForSubject(Subject* o)
{
	vector< shared_ptr<ProjectItemTabbedView> >::iterator it;
    for(it = mViews.begin(); it != mViews.end(); ++it)
    {
		shared_ptr<ProjectItemTabbedView> ptr = (*it);
        if(ptr)
        {
            if(ptr->getSubject() == o)
            {
                return ptr->getTabSheet();
            }
        }
    }
    return nullptr;
}

void TreeItemObservers::update(Subject* s, SubjectEvent se)
{
    if(se == SubjectEvent::SubjectBeingDestroyed)
    {
        removeViewForSubject(s);
        Log(lInfo) << "Observing: " <<s->getTag()<< ": " <<s->hasObserver(this);
	}

    if(mViews.size() == 0)
    {
        this->setInActive();
    }
    Log(lInfo) << "Number of views after update: " << mViews.size();
}


}
