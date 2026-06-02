#pragma once
#include "CJGW_FTM_SocketResponceVo.h"

namespace JGW
{
    class CCJGW_FTM_OTGInfooResponceVoJson : public CCJGW_FTM_SocketResponceVo
    {
    public:
        CCJGW_FTM_OTGInfooResponceVoJson(void);
        ~CCJGW_FTM_OTGInfooResponceVoJson(void);

        bool FromJosn(CCJGW_FTM_Json& ftmJson);
    public:
        bool otgMounted;
        long otgTotalBytes;
        long otgFreeBytes;
    };
}