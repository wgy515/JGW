#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "SCLLibHelper.h"
namespace JGW
{
    class CTSE_OpenTruntable : public CTSE_TestBase
    {
    public:
        CTSE_OpenTruntable(void);
        ~CTSE_OpenTruntable(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        int FindTruntableComPort();
        bool OpenTrunTable(int nComPort);
    private:
        int mnTimeOutSec;
        std::wstring mstrUsbServiceName;
        std::wstring mstrUsbVID;
        std::wstring mstrUsbPid;
        std::wstring mstrTruntableEnvironment;
        SCLLibHelper mSCLLibHelper;
    };
}
