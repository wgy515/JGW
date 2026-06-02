#pragma once
#include "CJGW_FTM_SocketResponceVo.h"

namespace JGW
{
    class CCJGW_FTM_QC3InfoResponceVoJson : public CCJGW_FTM_SocketResponceVo
    {
    public:
        CCJGW_FTM_QC3InfoResponceVoJson(void);
        ~CCJGW_FTM_QC3InfoResponceVoJson(void);

        bool FromJosn(CCJGW_FTM_Json& ftmJson);
    public:
        std::string mode;
    };
}


