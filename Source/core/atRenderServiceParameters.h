#ifndef atRenderServiceParametersH
#define atRenderServiceParametersH
#include "atRESTServiceParameters.h"
//---------------------------------------------------------------------------

namespace at
{

class ATE_CORE RenderServiceParameters : public RESTServiceParameters
{
    public:
                                    RenderServiceParameters();
                                    RenderServiceParameters(const string& host, int portNr = 80, const string& version = v1);
                                    ~RenderServiceParameters();

        void                        setMaxTilesToRender(int v);
        int                         getMaxTilesToRender() const;


    protected:

        int                         mMaxTilesToRender;

};
}

#endif
