#pragma once
#include "TSEServerBaseJson.h"
#include <map>

namespace JGW
{
    class CTSEControlCmdJson : public CTSEServerBaseJson
    {
    public:
        CTSEControlCmdJson(void);
        ~CTSEControlCmdJson(void);
    public:
        bool FromJosn(CCJGW_FTM_Json& msgJson);
        //StartFlag[] = L"{D261BC0C-45F3-498D-AEE8-5E730691CF35}";
        //szPassFlag[] = L"{A6A28E32-E91C-40D0-A79B-06CC8B441601}";
        //szFailFlag[] = L"{241BC6EF-0CD0-49CE-9B39-EEA67691669C}";
        //szExitFlag[] = L"{65FDA37A-8DFE-4540-9611-255A5C823649}";
        //! unknown -1,fail 0,pass 1,start 2,stop 3,exit 4
        TSETestControlEnum GetTestControlCmdIndex();
    public:
        std::string TestGuid;
        std::map<std::string,std::string> Params;
    };
}


