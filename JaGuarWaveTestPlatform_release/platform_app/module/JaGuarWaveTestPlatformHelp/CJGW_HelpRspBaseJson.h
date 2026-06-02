#pragma once
#include "CJGW_FTM_Json.h"

namespace JGW
{
    class CCJGW_HelpRspBaseJson
    {
    public:
        CCJGW_HelpRspBaseJson(void);
        ~CCJGW_HelpRspBaseJson(void);
    public:
        virtual bool FromJosnString(const std::string& strJson);
        virtual bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        bool bUpdate; //! true 需要更新 false 不需要更新
        bool status; //! 状态  true : false
        int helpRspType;
        std::string version;
        std::string error_msg;
    };
}