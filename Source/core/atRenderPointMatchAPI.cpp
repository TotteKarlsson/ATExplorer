#pragma hdrstop
#include "atRenderPointMatchAPI.h"
#include "atRenderClient.h"
#include "atPointMatchCollection.h"
#include "atRESTRequest.h"
#include "atRESTResponse.h"
#include "atJSONParser.h"
//---------------------------------------------------------------------------

namespace at
{

using namespace dsl;

RenderPointMatchAPI::RenderPointMatchAPI(RenderClient& rc)
:
RenderAPI("PointMatchAPI", rc)
{}

RenderPointMatchAPI::~RenderPointMatchAPI()
{}

RESTResponse* RenderPointMatchAPI::execute(RESTRequest& request)
{
    //Create a response object
    string result;
    int responseCode = mRC.executeRequest(request, result);
    Log(lDebug) << "HTTP Response Code: " << responseCode;
    return new RESTResponse(result, responseCode);
}

StringList RenderPointMatchAPI::getPointMatchCollectionOwners()
{
    RESTRequest request(mRC.getBaseURL(), rmGet);
    request.addSegment("matchCollectionOwners");
    RESTResponse* response = dynamic_cast<RESTResponse*>(execute(request));
    return (response) ?	response->getAsStringList() : StringList();
}

StringList RenderPointMatchAPI::getPointMatchCollectionNamesForOwner(const string& o)
{
	ListOfPointers<PointMatchCollection*> cs = getPointMatchCollectionsForOwner(o);

    StringList owners;
    PointMatchCollection* c = cs.getFirst();
    while(c)
    {
        owners.append(c->getName());
        c = cs.getNext();
    }
    return owners;
}

StringList RenderPointMatchAPI::getPointMatchGroupIDs(const string& o, const string& matchCollection)
{
    RESTRequest request(mRC.getBaseURL(), rmGet);
    request.addParameter("owner", o);
    request.addParameter("matchCollection", matchCollection);
    request.addSegment("groupIds");
    RESTResponse* response = dynamic_cast<RESTResponse*>(execute(request));
    return (response) ?	response->getAsStringList() : StringList();
}

StringList RenderPointMatchAPI::getPPointMatchGroupIDs(const string& o, const string& matchCollection)
{
	RESTRequest request(mRC.getBaseURL(), rmGet);
    request.addParameter("owner", o);
    request.addParameter("matchCollection", matchCollection);
    request.addSegment("pGroupIds");
    RESTResponse* response = dynamic_cast<RESTResponse*>(execute(request));
    return (response) ?	response->getAsStringList() : StringList();
}

StringList RenderPointMatchAPI::getQPointMatchGroupIDs(const string& o, const string& matchCollection)
{
	RESTRequest request(mRC.getBaseURL(), rmGet);
    request.addParameter("owner", o);
    request.addParameter("matchCollection", matchCollection);
    request.addSegment("qGroupIds");
    RESTResponse* response = dynamic_cast<RESTResponse*>(execute(request));
    return (response) ?	response->getAsStringList() : StringList();
}

ListOfObjects<PointMatch> RenderPointMatchAPI::getPQMatches(const string& o, const string& mc, const string& pGroup, const string& qGroup)
{
	RESTRequest request(mRC.getBaseURL(), rmGet);
    request.addParameter("owner", o);
    request.addParameter("matchCollection", mc);
    request.addParameter("group", pGroup);
    request.addParameter("matchesWith", qGroup);

    RESTResponse* response = dynamic_cast<RESTResponse*>(execute(request));

    string json = response->getContent();

    //Parse the response
	JSONParser parser(json);

    //Use our knowledge of the JSON..
 	vector<double>  w 	= parser.get1DDoubleArray("matches", "w");
 	vector<Point2D> p   = parser.get2DDoubleArray("matches", "p");
 	vector<Point2D> q   = parser.get2DDoubleArray("matches", "q");

    Log(lInfo) << w.size() << ":" <<p.size() << ":" << q.size();
    if((w.size() != p.size() || w.size() != q.size()))
    {
        Log(lError) << "Bad pointmatch data..";
		return ListOfObjects<PointMatch>();
    }

    //Create the list
	ListOfObjects<PointMatch> list;
    for(int point = 0; point < w.size(); point++)
    {
	    PointMatchPoint pmpP(pGroup, PointMatchPoint::pGroup, p[point], w[point]);
	    PointMatchPoint pmpQ(qGroup, PointMatchPoint::qGroup, q[point], w[point], &pmpP);
        pmpP.assignCorrespondingPoint(pmpQ);

        list.append(PointMatch(pmpP, pmpQ));
    }

    return list;
}

bool RenderPointMatchAPI::deletePointMatchCollection(const string& o, const string& mc)
{
	RESTRequest request(mRC.getBaseURL(), rmDelete);
    request.addParameter("owner", o);
    request.addParameter("matchCollection", mc);

    RESTResponse* response = dynamic_cast<RESTResponse*>(execute(request));
    string json = response->getContent();
    if(response)
    {
        switch(response->getResponseCode())
        {
            case 404:
                Log(lWarning) << "The collection: " << mc <<" was not found";
            return false;
        }
    }
    return false;
}


ListOfPointers<PointMatchCollection*> RenderPointMatchAPI::getPointMatchCollectionsForOwner(const string& o)
{
	RESTRequest request(mRC.getBaseURL(), rmGet);
    request.addParameter("owner", o);
    request.addSegment("matchCollections");
    RESTResponse* response = dynamic_cast<RESTResponse*>(execute(request));

    string json = response->getContent();

    //Parse the response
	JSONParser parser(json);

    //Parse each json object: schema
//[
//  {
//    "collectionId": {
//      "owner": "string",
//      "name": "string"
//    },
//    "pairCount": 0,
//    "owner": "string"
//  }
//]


    StringList list;
	if(response)
    {
    	list = response->getAsStringList();
    }

    //Put contexts in a list
    ListOfPointers<PointMatchCollection*> contexts;
    for(int i = 0; i < list.count(); i++)
    {
        PointMatchCollection* pc = new PointMatchCollection(o, list[i]);
        contexts.append(pc);
    }

    return contexts;
}

}
