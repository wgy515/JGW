#include "StdAfx.h"
#include "CJGW_QIARFTestHelp.h"
#include <process.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    //! stm ptr
    std::shared_ptr<void> gptrStmInterface;

    CCJGW_QIARFTestHelp::CCJGW_QIARFTestHelp(void) : mhExecutionEngineThreadHandle(NULL),mbIsEngineThreadRuning(false),m_dThreadID(-1)
    {
    }


    CCJGW_QIARFTestHelp::~CCJGW_QIARFTestHelp(void)
    {
        UninitializeQIARFTest();
    }

    void stm_release_suite_manager_interface_qia(void* pStmInterface)
    {
        if (NULL != pStmInterface)
        {
            stm_clear_assembly_tbl(pStmInterface);
            stm_release_suite_manager_interface(&pStmInterface);
            pStmInterface = NULL;
        }
    }

    bool CCJGW_QIARFTestHelp::InitializeQIARFTest(bool bIsRFCal,std::string& strAssemblyFilePath)
    {
        if (!gptrStmInterface)
        {
            gptrStmInterface.reset(stm_create_suite_manager_interface(),stm_release_suite_manager_interface_qia);
            //gptrStmInterface = std::make_shared<void *>(stm_create_suite_manager_interface(),stm_release_suite_manager_interface_qia);
            mptrStmInterface = gptrStmInterface;
            if (!!gptrStmInterface)
            {
                StmCreateApplication(bIsRFCal,strAssemblyFilePath);
            }
        }
        mptrStmInterface = gptrStmInterface;
        return !!mptrStmInterface;
    }

    void CCJGW_QIARFTestHelp::StopEngineThread()
    {
        if (mbIsEngineThreadRuning)
        {
            mbIsEngineThreadRuning = false;
            Sleep(2000);
            if (mbIsEngineThreadRuning && NULL != mhExecutionEngineThreadHandle)
            {
                DWORD dwExitCode = STILL_ACTIVE;
                GetExitCodeThread(mhExecutionEngineThreadHandle,&dwExitCode);
                if (dwExitCode == STILL_ACTIVE)
                {
                    TerminateThread(mhExecutionEngineThreadHandle,-1);
                }
                CloseHandle(mhExecutionEngineThreadHandle);
                mhExecutionEngineThreadHandle = NULL;
            }
        }    
    }

    void CCJGW_QIARFTestHelp::UninitializeQIARFTest()
    {
        mcQIARFTest.StopQIARFTest();
        StopEngineThread();
    }

    void CCJGW_QIARFTestHelp::StmCreateApplication(bool bIsRFCal,std::string& strAssemblyFilePath)
    {
        stm_add_reference_assembly(mptrStmInterface.get(),bIsRFCal ? "RFCAL" : "RFVERIFY" ,strAssemblyFilePath.c_str());
        stm_create_application(mptrStmInterface.get(),bIsRFCal ? "RFCAL" : "RFVERIFY" ,&msStmAssemblyInfo);
    }

    unsigned int __stdcall TestExecutionEngineThread_RF_TestServer(void *lpParam)
    {
        CCJGW_QIARFTestHelp* pQIARFTestHelp = (CCJGW_QIARFTestHelp*)lpParam;
        if (NULL != pQIARFTestHelp)
        {
            try
            {
                pQIARFTestHelp->ExecutionEngineTest();
            }
            catch (...)
            {
            	LogE(L"Execution QIA test unknown error");
            }
        }
        pQIARFTestHelp->SetEngineThreadIsRuning(false);
        return 0;
    }

    void CCJGW_QIARFTestHelp::SetEngineThreadIsRuning(bool bRuning /* = false */)
    {
        mbIsEngineThreadRuning = bRuning;
    }

    bool CCJGW_QIARFTestHelp::ExecutionEngineThread(const std::string& strXttFilePath,int nTimeOutSec /* = 60 * 6 */)
    {
        mbIsEngineThreadRuning = true;
        mbEngineTestResult = false;
        mstrXttFilePath = strXttFilePath;
        mhExecutionEngineThreadHandle = (HANDLE)_beginthreadex(NULL,0,TestExecutionEngineThread_RF_TestServer,this,0,&m_dThreadID);
        if (NULL == mhExecutionEngineThreadHandle || mhExecutionEngineThreadHandle == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        jgw_timer jt;
        while (jt.elapsed() < nTimeOutSec && mbIsEngineThreadRuning)
        {
            Sleep(500);
        }
        mcQIARFTest.StopQIARFTest();
        return mbEngineTestResult;
    }

    void CCJGW_QIARFTestHelp::ExecutionEngineTest()
    {
        mbEngineTestResult = mcQIARFTest.ExecuteQIARFTest(mptrStmInterface.get(),&msStmAssemblyInfo,mstrXttFilePath,DutTestEventHandler,this);
    }

    void CCJGW_QIARFTestHelp::DutTestEventHandler(void * context, int type, const char * msg)
    {
        std::wstring strTemp = JGW_A2W_A(msg);
        switch(type)
        {
        case E_RFCAL_QSEQ_DEBUG_CAL: // debug
            LogI(strTemp.c_str());
            break;
        case E_RFCAL_QSEQ_LOG_CAL: // log - > QSEQ LOG
            LogI(strTemp.c_str());
            JGW_EraseLastAndFristTwoCharsW(strTemp);
            //! psRFQualcommInterface->HandleCPKLogInfo(strTemp);
            break;
        case E_RFCAL_QSEQ_ERROR_CAL: // error
            LogE(strTemp.c_str());
            break;
        case E_RFCAL_QSEQ_TPL3_CAL: // tpl3
            break;
        case E_RFCAL_QSEQ_HTML_CAL: // html
            //psRFQualcommInterface->SaveHtmlLogging(strTemp);
            LogU(E_USER_DEFINE_HTMLFILEPATH,strTemp.c_str());
            break;
        default:
            break;
        }
    }

    void CCJGW_QIARFTestHelp::ClearGlobalVariable()
    {
        mcQIARFTest.ClearGlobalVariable();
    }

    void CCJGW_QIARFTestHelp::SetGlobalVariable(const std::string& gvName,const std::string& value)
    {
        mcQIARFTest.SetGlobalVariable(gvName,value);
    }

    std::string CCJGW_QIARFTestHelp::GetGlobalVariable(const std::string& gvName)
    {
        return mcQIARFTest.GetGlobalVariable(gvName);
    }
}
