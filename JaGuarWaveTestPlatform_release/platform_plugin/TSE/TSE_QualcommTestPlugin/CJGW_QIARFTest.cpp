#include "StdAfx.h"
#include "CJGW_QIARFTest.h"
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_QIARFTest::CCJGW_QIARFTest(void) : mbOpenedXtt(false),mbStopQIARFTest(false)
    {
    }


    CCJGW_QIARFTest::~CCJGW_QIARFTest(void)
    {
    }

    void CCJGW_QIARFTest::ClearGlobalVariable()
    {
        mmapGlobalVariable.clear();
    }

    std::string CCJGW_QIARFTest::GetGlobalVariable(const std::string& gvName)
    {
        if (mmapGlobalVariable.end() != mmapGlobalVariable.find(gvName))
        {
            return mmapGlobalVariable[gvName];
        }
        return "";
    }

    void CCJGW_QIARFTest::SetGlobalVariable(const std::string& gvName,const std::string& value)
    {
        mmapGlobalVariable[gvName] = value;
    }

    void CCJGW_QIARFTest::StopQIARFTest()
    {
        mbStopQIARFTest = true;
    }

    bool CCJGW_QIARFTest::ExecuteQIARFTest(void* gStmInterface,stm_assembly_info* pstmAssemblyInfo,const std::string& strXttFilePath,sti_sys_evt_handler pstmSysEvtHandler,void* pstmParam)
    {
        // Initialize COM library
        // Note to developer: Do not use CoInitialize(NULL) or CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) to initialize the COM library
        // Must use CONINIT_MULTITHREADED

        CoInitializeEx(NULL, COINIT_MULTITHREADED);
        mbStopQIARFTest = false;
        ISchedulerPtr schedulerPtr(__uuidof(QSPRScheduler));
        ITestInfoPtr testInfo;
        if (!mstrXttFilePath.compare(strXttFilePath))
        {
            mbOpenedXtt = false;
        }
        mstrXttFilePath = strXttFilePath;
        if (!mbOpenedXtt)
        {
            mptrTestTree = schedulerPtr->OpenXTT(strXttFilePath.c_str());
            mbOpenedXtt = true;
        }
        if (!mptrTestTree)
        {
            LogE_F("open xtt fail(%s)",mstrXttFilePath.c_str());
            return false;
        }

        schedulerPtr->SetGlobalVariable("XTT_PATH_PASSED_TO_TEST_SERVER", mstrXttFilePath.c_str());
        // This is the MOST important API to call.  It asks the QIA engine to run one node at a time.
        mptrTestTree->RunTreeInteractive();

        //::stm_set_environment_var(td->STM, td->rfverify, "SN", td->m_UUTID.c_str());
        //::stm_set_environment_var(td->STM, td->rfverify, "RunByTestServer", "1");
        for (std::map<std::string,std::string>::iterator it = mmapGlobalVariable.begin();
            it != mmapGlobalVariable.end();
            ++ it)
        {
            schedulerPtr->SetGlobalVariable(it->first.c_str(),it->second.c_str());
        }
        stm_register_system_event_handler(gStmInterface,pstmAssemblyInfo,pstmParam,pstmSysEvtHandler);
        stm_set_log_filename(gStmInterface,pstmAssemblyInfo,mstrXttFilePath.c_str());
        _bstr_t sRFCAL_DLL("SubSysRFCal_NET.dll");
        _bstr_t sMM_DLL("SubSysMultiMode_NET.dll");
        _bstr_t sQSEQ_DLL("QSEQ_DOTNET");
        bool rfverify_test_status = true;
        bool rfcal_test_status = true;
        int executionStatus = mptrTestTree->GetExecutionStatus();
        bool bTestResult = false;
        std::wstring strTestName;

        while (executionStatus != (int)Passed && executionStatus != (int)Failed && !mbStopQIARFTest)
        {
            while (executionStatus != (int)Waiting)
            {
                Sleep(1000);
                LogI_F("Waiting...");
                executionStatus = mptrTestTree->GetExecutionStatus();
                if (mbStopQIARFTest)
                {
                    LogE(L"stop qia rf test");
                    mbOpenedXtt = false;
                    executionStatus = Failed;
                    goto TEST_END;
                }
            }
            bTestResult = false;
            testInfo = mptrTestTree->GetNextTestInfo();
            int testID = 0;
            if(testInfo != 0)
            {
                std::string sDLLName(testInfo->GetDLLName(), SysStringLen(testInfo->GetDLLName()));

                std::string sRealName(testInfo->GetrealName(), SysStringLen(testInfo->GetrealName()));
                std::string sTestName(testInfo->GetTestName(),SysStringLen(testInfo->GetTestName()));

                strTestName = JGW_A2W(sTestName,CP_UTF8);
                TestStatus(strTestName.c_str(),E_RUN_TEST_STATUS);
                TestStatus(strTestName.c_str(),E_RUNING_TEST_STATUS);

                if(sRealName.find("QSEQ_DOTNETTest.QSEQ_DOTNETTests.LoadTestRecord") != std::string::npos)
                {
                    SAFEARRAY* Parameters = testInfo->GetParameters();
                    LPUNKNOWN* punks =  NULL;
                    HRESULT hr = SafeArrayAccessData(Parameters, (void**)&punks); // direct access to SA memory
                    if (SUCCEEDED(hr))
                    {
                        long lLBound, lUBound;  // get array bounds
                        SafeArrayGetLBound(Parameters, 1 , &lLBound);
                        SafeArrayGetUBound(Parameters, 1, &lUBound); 

                        long cElements = lUBound - lLBound + 1; 

                        // cElements is the number of Parameters.   Need to iterate the add parameter to test server
                        for(int p = 0; p < cElements; p++)
                        {
                            LPUNKNOWN punk = NULL;

                            int x = Parameters->cDims;
                            int y = Parameters->cbElements;

                            punk = punks[p];     // for VARIANTs: punk = punks[i].punkVal

                            if(punk)
                            {
                                IParameterInfoPtr paramInfo(punk);         // access the object via IOrder interface

                                std::string sParamName(paramInfo->GetParameterName(), SysStringLen(paramInfo->GetParameterName()));

                                if(sParamName == "outputSequenceFilePath")
                                {
                                    if (mmapGlobalVariable.end() != mmapGlobalVariable.find("outputSequenceFilePath"))
                                    {
                                        ::stm_set_environment_var(gStmInterface, pstmAssemblyInfo, "outputSequenceFilePath", mmapGlobalVariable["outputSequenceFilePath"].c_str());
                                    }
                                    else
                                    {
                                        ::stm_set_environment_var(gStmInterface, pstmAssemblyInfo, "outputSequenceFilePath", paramInfo->GetParameterValue());
                                    }
                                    
                                    break;
                                }
                            }
                        }
                        SafeArrayUnaccessData(Parameters);                  
                    }
                    SafeArrayDestroy(Parameters);

                    bTestResult = mptrTestTree->RunNextTest() == 1;
                }
                else if((sRealName.find("QSEQ_DOTNETTest.QSEQ_DOTNETTests.CheckPhoneCommunication") != std::string::npos) || (sRealName.find("QSEQ_DOTNETTest.QSEQ_DOTNETTests.LoadDeviceConfig") != std::string::npos))
                {
                    bTestResult = mptrTestTree->RunNextTest() == 1;
                }
                // If the test is from RFCal/RFVerify/QSEQ, then run it.
                else if((sDLLName.find(sQSEQ_DLL) != std::string::npos) || (sDLLName.find(sMM_DLL) != std::string::npos) || (sDLLName.find(sRFCAL_DLL)!= std::string::npos))
                {
                    // Create RF Verify test
                    if (sDLLName.find(sQSEQ_DLL) != std::string::npos || sDLLName.find(sMM_DLL) != std::string::npos)
                    {
                        // QSEQ Test Server DLL.  Need to convert all the names from QSEQ_DOTNETTest.QSEQ_DOTNETTests.SetUUTInfo to QSEQSetUUTInfo                    
                        if(sRealName.find("QSEQ_DOTNETTest.QSEQ_DOTNETTests.SetUUTInfo") != std::string::npos)
                        {
                            testID = stm_add_test(gStmInterface, pstmAssemblyInfo, testInfo->GetTestName(), "QSEQSetUUTInfo");
                        }
                        else if (sRealName.find("QSEQ_DOTNETTest.QSEQ_DOTNETTests.StartQSEQSession") != std::string::npos)
                        {
                            testID = stm_add_test(gStmInterface, pstmAssemblyInfo, testInfo->GetTestName(), "QSEQStartQSEQSession");
                        }
                        else if (sRealName.find("QSEQ_DOTNETTest.QSEQ_DOTNETTests.StartDefaultQSEQSession") != std::string::npos)
                        {
                            testID = stm_add_test(gStmInterface, pstmAssemblyInfo, testInfo->GetTestName(), "QSEQStartQSEQDefaultSession");
                        }
                        else if (sRealName.find("QSEQ_DOTNETTest.QSEQ_DOTNETTests.StopQSEQSession")!= std::string::npos)
                        {
                            testID = stm_add_test(gStmInterface, pstmAssemblyInfo, testInfo->GetTestName(), "QSEQStopQSEQSession");
                        }
                        else if (sRealName.find("QSEQ_DOTNETTest.QSEQ_DOTNETTests.ClearCumulatedSequence") != std::string::npos)
                        {
                            testID = stm_add_test(gStmInterface, pstmAssemblyInfo, testInfo->GetTestName(), "QSEQClearCumulatedSequence");
                        }
                        else if (sRealName.find("QSEQ_DOTNETTest.QSEQ_DOTNETTests.LoadSequenceFromXMLCumulatively") != std::string::npos)
                        {
                            testID = stm_add_test(gStmInterface, pstmAssemblyInfo, testInfo->GetTestName(), "QSEQLoadSequenceFromXMLCumulatively");
                        }
                        else if (sRealName.find("QSEQ_DOTNETTest.QSEQ_DOTNETTests.RunCumulatedSequence") != std::string::npos)
                        {
                            testID = stm_add_test(gStmInterface, pstmAssemblyInfo, testInfo->GetTestName(), "QSEQRunCumulatedSequence");
                        }
                        else if (sRealName.find("QSEQ_DOTNETTest.QSEQ_DOTNETTests.RunAllSequencesByTech") != std::string::npos)
                        {
                            testID = stm_add_test(gStmInterface, pstmAssemblyInfo, testInfo->GetTestName(), "QSEQRunAllSequencesByTech");
                        }
                        // MultiMode Server DLL tests
                        else
                        {
                            testID = stm_add_test(gStmInterface, pstmAssemblyInfo, testInfo->GetTestName(), testInfo->GetrealName());
                        }

                    }
                    //Create RF Cal Test
                    if (sDLLName.find(sRFCAL_DLL) != std::string::npos )
                        testID = stm_add_test(gStmInterface, pstmAssemblyInfo, testInfo->GetTestName(), testInfo->GetrealName());


                    int numOfParams = testInfo->GetParameters()->cbElements;


                    /*  This article shows how to access Safearray Interface pointers
                    http://stackoverflow.com/questions/12412533/convert-cast-safearray-of-iunknowns-to-an-iterable-array-of-interface-pointers
                    */

                    SAFEARRAY* Parameters = testInfo->GetParameters();
                    LPUNKNOWN* punks =  NULL;
                    HRESULT hr = SafeArrayAccessData(Parameters, (void**)&punks); // direct access to SA memory
                    if (SUCCEEDED(hr))
                    {
                        long lLBound, lUBound;  // get array bounds
                        SafeArrayGetLBound(Parameters, 1 , &lLBound);
                        SafeArrayGetUBound(Parameters, 1, &lUBound);

                        long cElements = lUBound - lLBound + 1; 

                        // cElements is the number of Parameters.   Need to iterate the add parameter to test server
                        for(int p = 0; p < cElements; p++)
                        {
                            LPUNKNOWN punk = NULL;

                            int x = Parameters->cDims;
                            int y = Parameters->cbElements;

                            punk = punks[p];     // for VARIANTs: punk = punks[i].punkVal

                            if(punk)
                            {
                                IParameterInfoPtr paramInfo(punk);         // access the object via IOrder interface

                                std::string sParamName(paramInfo->GetParameterName(), SysStringLen(paramInfo->GetParameterName()));

                                if ((sDLLName.find(sMM_DLL) != std::string::npos) || (sDLLName.find(sQSEQ_DLL) != std::string::npos))
                                {

                                    std::string sRealName(testInfo->GetrealName(), SysStringLen(testInfo->GetrealName()));


                                    stm_add_parameter(gStmInterface, pstmAssemblyInfo, testID, paramInfo->GetParameterName(), paramInfo->GetParameterValue(), paramInfo->GetupperLimit(), paramInfo->GetlowerLimit(), paramInfo->GetParamMode());

                                }
                                if (sDLLName.find(sRFCAL_DLL) != std::string::npos)
                                {    
                                    stm_add_parameter(gStmInterface, pstmAssemblyInfo,testID, paramInfo->GetParameterName(), paramInfo->GetParameterValue(), paramInfo->GetupperLimit(), paramInfo->GetlowerLimit(), paramInfo->GetParamMode());
                                }
                            }
                        }
                        SafeArrayUnaccessData(Parameters);                    
                    }
                    SafeArrayDestroy(Parameters);

                    // Execute the test with either RF Cal/ RF Verify or QSEQ Server
                    int TestResult = 1;

                    try
                    {
                        if ((sDLLName.find(sQSEQ_DLL) != std::string::npos) || (sDLLName.find(sMM_DLL) != std::string::npos))
                        {
                            TestResult = stm_execute_test_id(gStmInterface, pstmAssemblyInfo, testID);
                            rfverify_test_status &= (bool) TestResult;
                            if(rfverify_test_status == true)
                                schedulerPtr->SetGlobalVariable("RFVERIFY_STATUS", "1" );
                            else
                                schedulerPtr->SetGlobalVariable("RFVERIFY_STATUS", "0" );
                        }

                        if (sDLLName.find(sRFCAL_DLL) != std::string::npos)
                        {
                            TestResult = stm_execute_test_id(gStmInterface, pstmAssemblyInfo, testID);
                            rfcal_test_status &= (bool)TestResult;
                            if(rfcal_test_status == true)
                                schedulerPtr->SetGlobalVariable("RFCAL_STATUS", "1");
                            else
                                schedulerPtr->SetGlobalVariable("RFCAL_STATUS", "0");

                        }
                    }

                    catch (...) /*(Exception e)*/
                    {
                        //rfverify_test_status &= TestResult = 0;
                        //ui.WriteLine(m_DUT_Index, MainForm.TargetWindow.KERNEL, info.TestName + " Throws Exceptions " + e.Message, false);
                    }

                    // Updating the scheduler on the test result
                    //CString *sMsg = new CString(_T("RFCal_Sleep: Passed\r\n"));
                    ////char* sTempMsg = new char[256];
                    if (TestResult == 1)
                    {
                    //    sMsg->Format(_T("%s :Passed\r\n"), testInfo->GetTestName().GetBSTR());
                        mptrTestTree->SetNextTestResult((int)TestPassed);
                        bTestResult = true;
                        //TestStatus(strTestName.c_str(),E_PASS_TEST_STATUS);
                    }
                    else
                    {
                    //    sMsg->Format(_T("%s :Failed\r\n"), testInfo->GetTestName().GetBSTR());
                        mptrTestTree->SetNextTestResult((int)TestFailed);
                        bTestResult = false;
                        //TestStatus(strTestName.c_str(),E_FAIL_TEST_STATUS);
                    }

                    //// Send the test result back to UI
                    //if(td->m_DUT_Index == 1)
                    //    AfxGetApp()->m_pMainWnd->PostMessage(UWM_PRINT_DUT1_PROGRESS_MSG, 0, (LPARAM) sMsg);
                    //else  if(td->m_DUT_Index == 2)
                    //    AfxGetApp()->m_pMainWnd->PostMessage(UWM_PRINT_DUT2_PROGRESS_MSG, 0, (LPARAM) sMsg);
                }
                else// else It is not a RF server test, Ask QSPR to run it
                {
                    bTestResult = mptrTestTree->RunNextTest() == 1;
                }
            }
            executionStatus = mptrTestTree->GetExecutionStatus();
            TestStatus(strTestName.c_str(),bTestResult ? E_FAIL_TEST_STATUS:E_PASS_TEST_STATUS);
        }
        /*if (executionStatus == (int)Failed)
        {
            std::string strLastError(mptrTestTree->GetLastError(),SysStringLen(mptrTestTree->GetLastError()));
            LogE_F(strLastError.c_str());
        }*/
TEST_END:
        stm_clear_test_db(gStmInterface, pstmAssemblyInfo);
        ::CoUninitialize();
        return executionStatus == Passed;
    }
}

