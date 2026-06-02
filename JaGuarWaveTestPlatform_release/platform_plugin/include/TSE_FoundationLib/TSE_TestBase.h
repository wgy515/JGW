#pragma once
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include "TSE_FoundationLib_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#define TSE_PARAM_NAME_EQUAL(ParamName,strParamName) (0 == JGW_WStrComparenoCaseWStr(ParamName,strParamName))

namespace JGW
{
    class CTSE_TestBase
    {
    public:
        CTSE_TestBase();
        virtual ~CTSE_TestBase();
        //! 获取参数配置的相关描述讲解
        virtual const wchar_t* TSE_GetParamDescription();
        //! Unknown Attribute
        virtual bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        //! 
        virtual bool TSE_Init(void);
        //! 
        virtual bool TSE_Run(void);
        //! 
        virtual bool TSE_Exit(void);

        virtual size_t TSE_GetErrorRetryTestCount(void);
    public:
        virtual std::wstring& TSE_GetTestName();
    protected:
        size_t mnIntervalTimeMS;
        size_t mnErrorRetryCount;
        std::wstring mstrTestName;
    };
}