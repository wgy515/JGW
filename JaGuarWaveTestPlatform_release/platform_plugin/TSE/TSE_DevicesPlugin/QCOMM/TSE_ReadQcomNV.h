#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "PacketTranslator.h"
namespace JGW
{
    class CTSE_ReadQcomNV:public CTSE_TestBase
    {
    public:
        CTSE_ReadQcomNV(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        CPacketTranslator mPacketFormatter;
        unsigned char* mpData;
        unsigned long mPacketLen;
        unsigned short mitemID;
        unsigned short miSub_ID;
    };
}


