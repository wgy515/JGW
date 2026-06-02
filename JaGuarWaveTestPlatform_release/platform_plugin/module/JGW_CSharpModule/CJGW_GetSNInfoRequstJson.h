#pragma once

namespace JGW
{
    class CCJGW_GetSNInfoRequstJson
    {
    public:
        CCJGW_GetSNInfoRequstJson(void);
        ~CCJGW_GetSNInfoRequstJson(void);
    public:
        bool IsSMTMoNo;
        std::string SN;
        std::string OperateID;
    };

}

