#include "StdAfx.h"
#include "CProductScreenThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_WindowsFuncPlugin/PerasoDefine.h>

namespace JGW
{
    CCProductScreenThread::CCProductScreenThread(void):mstrSerialNumber(_T(""))
    {
        mszTestResultBuf = (char*)calloc(0x01,PRODUCT_SCREEN_TEST_RESULT_TXT_MAX_LEN);
    }


    CCProductScreenThread::~CCProductScreenThread(void)
    {
        free(mszTestResultBuf);
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCProductScreenThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCProductScreenThread::OnInitThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCProductScreenThread::OnRunThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCProductScreenThread::OnInitThread( WPARAM wParam,LPARAM lParam )
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsProductScreenParam = (LPS_PRODUCT_SCREEN_PARAM)lParam;
    }

    void CCProductScreenThread::OnRunThread(WPARAM wParam,LPARAM lParam)
    {
        SendTestMsgToWnd(L"Start Test ...\n");
//         if (!mbInitLoadPDBImage)
//         {
//             SendTestMsgToWnd(L"Load PDB Image FAIL....\nPlease check pdf folder exists\n");
//             mpAsyncWndMessage->put_async_message(WM_TEST_FAIL_MSG);
//             return;
//         }
#if 1
        if (!FindPerasoComPort()
            || !InitProductFolderPath()
            || !ExecuteProductScreenTest()
            || !ReadProductScreenTestResult()
            || !CheckTestIsOk()
            || !GetSerailNumberToResultText()
            || !SaveTestLogFile()
            )
        {
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
#else
        if (!InitProductFolderPath()
            || !SaveTestLogFile()
            )
        {
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
#endif
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
        SendTestMsgToWnd(L"Test Finished ok");
    }

    void CCProductScreenThread::SendTestMsgToWnd(const wchar_t* strMsg)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strMsg,NULL);
    }

    bool CCProductScreenThread::FindPerasoComPort()
    {
        std::wstring strComPort;
        for (int i = 0;i < 50;i ++)
        {
            SendTestMsgToWnd(L"Find Device ...\n");
            strComPort = JGW_GetComPortToUsbServiceName(L"usbser",L"VID_2932");
            if (!strComPort.empty()) 
            { 
                SendTestMsgToWnd(L"Found Device\n");
                return true;
            }
            Sleep(1000); 
        }
        SendTestMsgToWnd(L"No Found Device\n");

        return false;
    }

    bool CCProductScreenThread::InitProductFolderPath()
    {
        std::wstring strTemp;
        while (JGW_FileExistsToFilePath(mpsProductScreenParam->mstrReportXmlFilePath.c_str()))
        {
            JGW_FormatWString(strTemp,L"Deletint XML Report file : %s\n",mpsProductScreenParam->mstrReportXmlFilePath.c_str());
            SendTestMsgToWnd(strTemp.c_str());
            DeleteFile(mpsProductScreenParam->mstrReportXmlFilePath.c_str());
        }

        while (JGW_FileExistsToFilePath(mpsProductScreenParam->mstrTestResultFilePath.c_str()))
        {
            JGW_FormatWString(strTemp,L"Deletint Measurements file : %s\n",mpsProductScreenParam->mstrReportXmlFilePath.c_str());
            SendTestMsgToWnd(strTemp.c_str());
            DeleteFile(mpsProductScreenParam->mstrTestResultFilePath.c_str());
        }

        while (JGW_FileExistsToFilePath(mpsProductScreenParam->mstrVerboseLogFilePath.c_str()))
        {
            JGW_FormatWString(strTemp,L"Deletint Verbose log file : %s\n",mpsProductScreenParam->mstrReportXmlFilePath.c_str());
            SendTestMsgToWnd(strTemp.c_str());
            DeleteFile(mpsProductScreenParam->mstrVerboseLogFilePath.c_str());
        }

        return true;
    }
    //! Finished test session gracefully
    bool CCProductScreenThread::ExecuteProductScreenTest()
    {
        std::wstring strCmdLine = _T("ProductScreenTool.exe -o testresult.txt -p 3\n");
        int recv_count = 0;

        if (!mcProecessPide.CheckProcessIsRun())
        {
            std::wstring strTemp;
            if (!mcProecessPide.InitProcessPipe(mpsProductScreenParam->mstrCMDFilePath.c_str(),strTemp,mpsProductScreenParam->mstrProductScreenToolFolder))
            {
                SendTestMsgToWnd(L"ProductScreenTool.exe Execute Failed\n");
                return false;
            }
            while (1)
            {
                recv_count = mcProecessPide.recv(strTemp);
                if (-1 == recv_count)
                {
                    SendTestMsgToWnd(L"未知异常错误信息\n");
                    mcProecessPide.CloseProcessPipe();
                    return false;
                }
                else if (0 == recv_count)
                {
                    Sleep(100);
                    continue;
                }
                else
                {
                    if (L'>' == strTemp.at(strTemp.size() - 1))
                    {
                        break;
                    }
                }
            }   
        }

        if (!mcProecessPide.send(strCmdLine))
        {
            SendTestMsgToWnd(L"Send Command To cmd.exe Failed\n");
            return false;
        }


        return true;
    }

    bool CCProductScreenThread::ReadProductScreenTestResult()
    {
        std::wstring strRead;
        mstrReadTestResult = L"";
        int recv_count = 0;
        jgw_timer jgw_time;

        SendTestMsgToWnd(L"Start recv ProductScreenTest Result......\n");
        while (PRODUCT_SCREEN_TEST_TIME_OUT_SEC >= jgw_time.elapsed())
        {
            recv_count = mcProecessPide.recv(strRead);
            if (-1 == recv_count)
            {
                SendTestMsgToWnd(L"未知异常错误信息\n");
                mcProecessPide.CloseProcessPipe();
                return false;
            }
            else if (0 == recv_count)
            {
                Sleep(100);
                continue;
            }
            else
            {
                mstrReadTestResult += strRead;
                SendTestMsgToWnd(strRead.c_str());
                if (L'>' == strRead.at(strRead.size() - 1))
                {
                    return true;
                }
            }
        }
        SendTestMsgToWnd(L"recv time out\n");
        return false;
    }

    bool CCProductScreenThread::CheckTestIsOk()
    {
        if (std::wstring::npos == mstrReadTestResult.find(L"Finished test session gracefully"))
        {
            SendTestMsgToWnd(L"Product Screen Test Failed\n");
            return false;
        }
        //! [  FAILED  ] 0 tests.
        if (std::wstring::npos != mstrReadTestResult.find(L"PASSED"))
        {
            return true;
        }

        

//         //! 
//         if (std::wstring::npos == mstrReadTestResult.find(L"[  FAILED  ] 1 tests, listed below:"))
//         {
//             return false;
//         }
// 
        if (std::wstring::npos != mstrReadTestResult.find(L"0 failed tests"))
        {
            return true;
        }

        return false;
    }

    bool CCProductScreenThread::GetSerailNumberToResultText()
    {
        SendTestMsgToWnd(L"Get Serail Number To testresult.txt\n");
        std::string strTestResultFilePath = JGW_W2A(mpsProductScreenParam->mstrTestResultFilePath);
        size_t buf_len = PRODUCT_SCREEN_TEST_RESULT_TXT_MAX_LEN;
        memset(mszTestResultBuf,0x00,PRODUCT_SCREEN_TEST_RESULT_TXT_MAX_LEN);
        char* pBuf = NULL;
        char szSerialNumber[30] = {0};

        if (!JGW_GetFileBufToPath_BufLen_C(strTestResultFilePath.c_str(),mszTestResultBuf,buf_len))
        {
            SendTestMsgToWnd(L"Open testresult.txt Failed\n");
            return false;
        }

        if (PRODUCT_SCREEN_TEST_RESULT_TXT_MAX_LEN < buf_len)
        {
            SendTestMsgToWnd(L"testresult.txt file size Too Large\n");
            return false;
        }

        pBuf = strstr(mszTestResultBuf,"2.3.9,");
        if (NULL == pBuf)
        {
            SendTestMsgToWnd(L"Get Serail Number Failed\n");
            return false;
        }
        pBuf += strlen("2.3.9,");

        for (int i = 0;i < 29;i ++)
        {
            if (',' == *pBuf) break;

            szSerialNumber[i] = *pBuf;
            ++ pBuf;
        }
        /*
        pTemp = strstr(pBuf,",");
        if (29 < pTemp - pBuf)
        {
            SendTestMsgToWnd(L"Get Serail Number Too Large");
            return false;
        }
        memcpy(pBuf,szSerialNumber,pTemp - pBuf);*/
        mstrSerialNumber = JGW_A2W_A(szSerialNumber);
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"Serail Number : %s\n",mstrSerialNumber.c_str());
        SendTestMsgToWnd(strTemp.c_str());

        if (SERIALNUMBER_MAX_LEN != mstrSerialNumber.length())
        {
            SendTestMsgToWnd(L"Get Serial Number Len != 14");
            return false;
        }
        return true;
    }

    bool CCProductScreenThread::SaveTestLogFile()
    {
        std::wstring strFolder,strTestResultFilePath,strVerboseLogFilePath,strReportXmlFilePath,strTemp;
        //!MoveFile
        JGW_FormatWString(strFolder,L"%s%s_%d\\",mpsProductScreenParam->mstrTestLogFolder.c_str(),mstrSerialNumber.c_str(),GetTickCount());
        JGW_CreateDirectory(strFolder.c_str(),true);
        JGW_FormatWString(strTemp,L"Save Test Log Path: %s\n",strFolder.c_str());
        SendTestMsgToWnd(strTemp.c_str());

        strTestResultFilePath =  strFolder + _T(PRODUCT_SCREEN_TEST_RESULT_TXT);
        strVerboseLogFilePath = strFolder + _T(PRODUCT_SCREEN_VERBOSE_LOG_FILE);
        strReportXmlFilePath = strFolder + _T(PRODUCT_SCREEN_XML_REPORT_FILE);
        
        JGW_FormatWString(strTemp,L"\"%s\" Move To \"%s\"\n",mpsProductScreenParam->mstrReportXmlFilePath.c_str(),strReportXmlFilePath.c_str());
        SendTestMsgToWnd(strTemp.c_str());
        if (!MoveFileEx(mpsProductScreenParam->mstrReportXmlFilePath.c_str(),strReportXmlFilePath.c_str(),MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
        {
            SendTestMsgToWnd(L"Move File Fail\n");
            return false;
        }

        JGW_FormatWString(strTemp,L"\"%s\" Move To \"%s\"\n",mpsProductScreenParam->mstrTestResultFilePath.c_str(),strTestResultFilePath.c_str());
        SendTestMsgToWnd(strTemp.c_str());
        if (!MoveFileEx(mpsProductScreenParam->mstrTestResultFilePath.c_str(),strTestResultFilePath.c_str(),MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
        {
            SendTestMsgToWnd(L"Move File Fail\n");
                return false;
        }

        JGW_FormatWString(strTemp,L"\"%s\" Move To \"%s\"\n",mpsProductScreenParam->mstrVerboseLogFilePath.c_str(),strVerboseLogFilePath.c_str());
        SendTestMsgToWnd(strTemp.c_str());
        if (!MoveFileEx(mpsProductScreenParam->mstrVerboseLogFilePath.c_str(),strVerboseLogFilePath.c_str(),MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
        {
            SendTestMsgToWnd(L"Move File Fail\n");
            return false;
        }

        return true;
    }
}

