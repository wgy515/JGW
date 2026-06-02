#pragma once
#include "CJGW_FTM_SocketResponceVo.h"

namespace JGW
{
    struct S_MEMORY_CARD_INFO 
    {   
        __int64 totalBlocks;
        __int64 freeBlocks;
        __int64 blockByteSize;
        __int64 availableBlocks;
        __int64 availableBytes;
        __int64 totalBytes;
        __int64 freeBytes;
        std::string description;
    };
    class CCJGW_FTM_MemoryCardResponceVoJson : public CCJGW_FTM_SocketResponceVo
    {
    public:
        CCJGW_FTM_MemoryCardResponceVoJson(void);
        ~CCJGW_FTM_MemoryCardResponceVoJson(void);

        bool FromJosn(CCJGW_FTM_Json& ftmJson);
    public:
         std::vector<S_MEMORY_CARD_INFO> mvsMemoryCardInfo;
    };
}

