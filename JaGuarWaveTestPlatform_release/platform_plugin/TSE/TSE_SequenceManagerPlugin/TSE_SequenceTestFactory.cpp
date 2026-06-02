#include "StdAfx.h"
#include "TSE_SequenceTestFactory.h"
#include "TSE_SequenceTest.h"
#include "tse2/TSE2_SequenceTest.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CTSE_SequenceTestFactory::CTSE_SequenceTestFactory(CTSE_LogServices& logServices,CTSE_TestStatusService& testStatusService,CTSE_GlobalEnvironment& globalEnvironment,int token_id) : mpSequenceTestImpl(NULL),mLogServices(logServices),mTestStatusService(testStatusService),mGlobalEnvironment(globalEnvironment),mnTokenID(token_id)
    {
    }


    CTSE_SequenceTestFactory::~CTSE_SequenceTestFactory(void)
    {
        if (mpSequenceTestImpl)
        {
            delete mpSequenceTestImpl;
            mpSequenceTestImpl = NULL;
        }
    }

    bool CTSE_SequenceTestFactory::LoadTSESuiteConfig(const std::string& strResourceConfig)
    {
        if (NULL == mpSequenceTestImpl)
        {
            //! <RootTest>
            if (NULL != JGW_StrCaseStr(strResourceConfig.c_str(),"_tse2.") || NULL != JGW_StrCaseStr(strResourceConfig.c_str(),"<RootTest>"))
            {
                mpSequenceTestImpl = new CTSE2_SequenceTest(mLogServices,mTestStatusService,mGlobalEnvironment,mnTokenID);
            }
            else
            {
                mpSequenceTestImpl = new CTSE_SequenceTest(mLogServices,mTestStatusService,mGlobalEnvironment,mnTokenID);
            }
        }
        return mpSequenceTestImpl->LoadTSESuiteConfig(strResourceConfig);
    }

    bool CTSE_SequenceTestFactory::ExecTSEAllSuiteTest(bool bErrorStop /* = true */)
    {
        if (NULL != mpSequenceTestImpl)
        {
            return mpSequenceTestImpl->ExecTSEAllSuiteTest(bErrorStop);
        }
        return false;
    }

    bool CTSE_SequenceTestFactory::ExecTSEConfigTest()
    {
        if (NULL != mpSequenceTestImpl)
        {
            return mpSequenceTestImpl->ExecTSEConfigTest();
        }
        return false;
    }

    bool CTSE_SequenceTestFactory::ExecTSEAutoTest()
    {
        if (NULL != mpSequenceTestImpl)
        {
            return mpSequenceTestImpl->ExecTSEAutoTest();
        }
        return false;
    }

    bool CTSE_SequenceTestFactory::ExecTSETestToIndex(size_t index)
    {
        if (NULL != mpSequenceTestImpl)
        {
            return mpSequenceTestImpl->ExecTSETestToIndex(index);
        }
        return false;
    }

    bool CTSE_SequenceTestFactory::ExecTSEGroupTestToIndex(size_t index)
    {
        if (NULL != mpSequenceTestImpl)
        {
            return mpSequenceTestImpl->ExecTSEGroupTestToIndex(index);
        }
        return false;
    }

    bool CTSE_SequenceTestFactory::ExecTSETestFinalize(void)
    {
        if (NULL != mpSequenceTestImpl)
        {
            return mpSequenceTestImpl->ExecTSETestFinalize();
        }
        return false;
    }

    bool CTSE_SequenceTestFactory::InitTSETestConfig(void)
    {
        if (NULL != mpSequenceTestImpl)
        {
            return mpSequenceTestImpl->InitTSETestConfig();
        }
        return false;
    }

    bool CTSE_SequenceTestFactory::TSEAutoTestNotEmpty()
    {
        if (NULL != mpSequenceTestImpl)
        {
            return mpSequenceTestImpl->TSEAutoTestNotEmpty();
        }
        return false;
    }

    void CTSE_SequenceTestFactory::ClearTSEExistedTestArrayIndex()
    {
        if (NULL != mpSequenceTestImpl)
        {
            mpSequenceTestImpl->ClearTSEExistedTestArrayIndex();
        }
    }

    const wchar_t* CTSE_SequenceTestFactory::GetTSETestNameToIndex(size_t nIndex)
    {
        if (NULL != mpSequenceTestImpl)
        {
            return mpSequenceTestImpl->GetTSETestNameToIndex(nIndex);
        }
        return L"";
    }

    void CTSE_SequenceTestFactory::UnLoadTSETestPlugin()
    {
        if (NULL != mpSequenceTestImpl)
        {
            mpSequenceTestImpl->UnLoadTSETestPlugin();
        }
    }
}
