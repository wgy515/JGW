#pragma once
#include <JGW_TestSuiteManagePlugin/CJGW_TestSuiteImpl.h>
#include "PacketTranslator.h"
namespace JGW
{
    class CCJGW_ReadQcomNV:public CCJGW_TestSuiteImpl
    {
    public:
        CCJGW_ReadQcomNV(void);
    private:
        bool CreateParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool Run();
    private:
        CPacketTranslator mPacketFormatter;
        unsigned char* mpData;
        unsigned long mPacketLen;
        unsigned short mitemID;
        unsigned short miSub_ID;
    };
}


