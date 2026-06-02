#include "StdAfx.h"
#include "CJGW_QIATest.h"
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    CCJGW_QIATest::CCJGW_QIATest(void) : mbOpenTestTree(false)
    {
    }


    CCJGW_QIATest::~CCJGW_QIATest(void)
    {
    }

    void CCJGW_QIATest::ClearGlobalVariable()
    {
        mmapGlobalVariable.clear();
    }

    std::string CCJGW_QIATest::GetGlobalVariable(const std::string& gvName)
    {
        if (mmapGlobalVariable.end() != mmapGlobalVariable.find(gvName))
        {
            return mmapGlobalVariable[gvName];
        }
        return "";
    }

    void CCJGW_QIATest::SetGlobalVariable(const std::string& gvName,const std::string& value)
    {
        mmapGlobalVariable[gvName] = value;
    }

    void CCJGW_QIATest::SetTestXttPath(const std::string& strXttFilePath)
    {
        mstrXttFilePath = strXttFilePath;
    }

    bool CCJGW_QIATest::ExecuteXttTest()
    {
        CoInitializeEx(NULL, COINIT_MULTITHREADED);
        ISchedulerPtr schedulerPtr(__uuidof(QSPRScheduler));  
        ITestInfoPtr testInfo;    

        if(!mbOpenTestTree)
        {
            bstr_t XTT(mstrXttFilePath.c_str());
            mptrITestTreePtr = schedulerPtr->OpenXTT(XTT);
            mbOpenTestTree = true;
        }
        schedulerPtr->SetGlobalVariable("XTT_PATH_PASSED_TO_TEST_SERVER", mstrXttFilePath.c_str());

        ITestTreePtr xtt = mptrITestTreePtr;    

        // This is the MOST important API to call.  It asks the QIA engine to run one node at a time.
        xtt->RunTreeInteractive();

        int executionStatus = xtt->GetExecutionStatus();
        for (std::map<std::string,std::string>::iterator it = mmapGlobalVariable.begin();
            it != mmapGlobalVariable.end();
            ++ it)
        {
            schedulerPtr->SetGlobalVariable(it->first.c_str(),it->second.c_str());
        }

        while (executionStatus != (int)Passed && executionStatus != (int)Failed)
        {
            while (executionStatus != (int)Waiting)
            {
                Sleep(1000);
                LogI(L"Waiting...");
                executionStatus = xtt->GetExecutionStatus();
            }

            testInfo = xtt->GetNextTestInfo();
            if (testInfo != 0)
            {
                xtt->RunNextTest();
            }
            executionStatus = xtt->GetExecutionStatus();
        }

        ::CoUninitialize();
        return executionStatus == Passed;
    }
}
