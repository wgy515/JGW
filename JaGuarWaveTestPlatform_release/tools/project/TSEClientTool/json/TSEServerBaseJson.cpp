#include "StdAfx.h"
#include "TSEServerBaseJson.h"

namespace JGW
{
    CTSEServerBaseJson::CTSEServerBaseJson(void) : ActionEnumID(Unknown_AJE)
    {
    }


    CTSEServerBaseJson::~CTSEServerBaseJson(void)
    {
    }

    bool CTSEServerBaseJson::FromJosn(CCJGW_FTM_Json& msgJson)
    {
        int actionEnumID = Unknown_AJE;
        bool ret = msgJson.GetJsonValueToKey<int>("ActionEnumID",actionEnumID);
        ActionEnumID = (ActionJsonEnum)actionEnumID;
        return ret;
    }
}
