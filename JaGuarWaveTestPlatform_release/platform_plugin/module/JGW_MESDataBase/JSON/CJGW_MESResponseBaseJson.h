#pragma once
#include "CJGW_FTM_Json.h"

namespace JGW
{
    class CCJGW_MESResponseBaseJson
    {
    public:
        CCJGW_MESResponseBaseJson(void);
        ~CCJGW_MESResponseBaseJson(void);
        bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        //! 响应状态
        bool bStatus;
        //! 响应消息类型
        int responseType;
        //! 返回消息
        std::string returnMessage;
    };
}