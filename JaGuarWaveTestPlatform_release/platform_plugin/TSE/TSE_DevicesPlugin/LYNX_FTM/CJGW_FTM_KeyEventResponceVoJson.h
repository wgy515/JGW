#pragma once
#include "CJGW_FTM_SocketResponceVo.h"

namespace JGW
{
    class CCJGW_FTM_KeyEventResponceVoJson : CCJGW_FTM_SocketResponceVo
    {
    public:
        bool FromJosn(CCJGW_FTM_Json& ftmJson);
    public:
        std::string keyType;
        std::string event;
    };
}


