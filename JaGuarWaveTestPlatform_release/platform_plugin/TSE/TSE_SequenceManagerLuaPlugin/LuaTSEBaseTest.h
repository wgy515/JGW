#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CLuaTSEBaseTest
    {
    public:
        CLuaTSEBaseTest(void);
        ~CLuaTSEBaseTest(void);
    public:
        //!
        void SettingTSETestBase(CTSE_TestBase* pTestBase);
        //! Unknown Attribute
        virtual bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        //! 
        virtual bool TSE_Init(void);
        //! 
        virtual bool TSE_Run(void);
        //! 
        virtual bool TSE_Exit(void);

        virtual int TSE_GetErrorRetryTestCount();
    private:
        CTSE_TestBase* mpTestBase;
    };
}


