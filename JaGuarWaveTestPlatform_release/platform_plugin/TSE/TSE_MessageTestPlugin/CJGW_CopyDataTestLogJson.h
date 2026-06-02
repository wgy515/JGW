#pragma once
#include "CJGW_FTM_Json.h"
#include <string>
namespace JGW
{
    class CCJGW_CopyDataTestLogJson
    {
    public:
        CCJGW_CopyDataTestLogJson(void);
        ~CCJGW_CopyDataTestLogJson(void);

        bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        int LogType;
        int UserDefineID;
        std::wstring Message;
    };


}
