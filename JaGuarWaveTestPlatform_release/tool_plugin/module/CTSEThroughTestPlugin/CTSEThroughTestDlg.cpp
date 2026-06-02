#include "StdAfx.h"
#include "CTSEThroughTestDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CTSEThroughputTestThread.h"
#include <JGW_TestLoggingPlugin/CJGW_SaveTestLog.h>
#include <fstream>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>

//#include "CJGW_FTM_Json.h"
//#include "CJGW_CopyDataTestLogJson.h"

namespace JGW
{
    CCTSEThroughTestDlg::CCTSEThroughTestDlg(void) : mpTestThread(NULL)/*,mstrSaveOutPutLog(L"c:\\iperf.txt")*/,bUpdateLogTime(false)
    {
        msPluginSkin.mstrSkinXmlPath = L"tse_through_plugin_skin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
        mbIsShowLogHwnd = false;
    }

    CCTSEThroughTestDlg::~CCTSEThroughTestDlg(void)
    {
    }

    void CCTSEThroughTestDlg::CloseSubWndPlugin()
    {
        if (mpTestThread)
        {
            //! mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CLEANUP);
            //! Sleep(2 * 1000);
            mpTestThread->TerminateMessageThread();
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    void CCTSEThroughTestDlg::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
    }

    bool CCTSEThroughTestDlg::OnIdelTestStatus()
    {
        mpDoMainList = static_cast<CListTestCtrlUI*>(mpPluginPaintManagerUI->FindControl(L"domainlist"));
        mpThroughtputTestEdt = static_cast<CRichEditUI*>(mpPluginPaintManagerUI->FindControl(L"throughtputTestEdt"));
        if (!mpDoMainList) return false;
        {
            mpDoMainList->SetAttribute(L"itemalign",L"left");
            mpDoMainList->GetList()->RemoveAll();
            mpDoMainList->GetHeader()->SetScaleHeader(false);
            mpDoMainList->GetHeader()->RemoveAll();

            mpDoMainList->InsertColumn(0,_T("Index"),60);
            mpDoMainList->InsertColumn(1,_T("Test Project"),0);
            mpDoMainList->InsertColumn(2,_T("Content"),200);
            mpDoMainList->InsertColumn(3,_T("Time"),80);
            mpDoMainList->InsertColumn(4,_T("P/F"),40);
        }
        //! Init Param
        {
            msTSEThroughputTestConfigParam.mnTestTime = 30;
            msTSEThroughputTestConfigParam.mnTestTimeIndex = 0;
            msTSEThroughputTestConfigParam.mbMultThread = false; //! 是否是多路TCP测试
            msTSEThroughputTestConfigParam.mhMainWnd = mpPluginPaintManagerUI->GetPaintWindow();
            msTSEThroughputTestConfigParam.mstrSuiteXmlConfigPath = JGW_GetTSEConfigFilePath(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpViewResolverParam->mpPluginInfo->mstrSuiteXmlName);
        }

        {
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TSE_SHOW_CONTENT,&CCTSEThroughTestDlg::OnTSEShowTestContent);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_LIST_MSG,&CCTSEThroughTestDlg::OnTestListMsg);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_INIT_TSE_TEST_FAIL_MSG,&CCTSEThroughTestDlg::OnTSEInitFail);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_RICHEDIT_APPEND_TEST_MSG,&CCTSEThroughTestDlg::OnAppendRichEditTest);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_COPYDATA,&CCTSEThroughTestDlg::OnTSEAdapterLog);
        }
        //! Init Test Thread
        {
            if (!mpTestThread) mpTestThread = new CCTSEThroughputTestThread;
            if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

            return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&msTSEThroughputTestConfigParam));
        }   

        return true;
    }

    void CCTSEThroughTestDlg::OnRspTimeMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (WM_TIMER_UPDATE_LOG_INFO == wParam)
        {
            //OnTimerUpdateLogInfo();
        }
        else
        {
            CCJGW_CommanTestPlnView::OnRspTimeMsg(wParam,lParam);
        }
    }

    bool CCTSEThroughTestDlg::OnBusyTestStatus()
    {
        //! if (bUpdateLogTime) KillTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO);
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"Is Save Test Log : %s LF\r...",mbIsSaveTestLog ? L"true" : L"false");
        
        mstrTestLogStream.str(L"");
        mstrTestLogStream.clear();
        msTSEThroughputTestConfigParam.mnSeekgLog = 0;
        msTSEThroughputTestConfigParam.mnTestTimeIndex = 0;
        mstrPrevTestProjectName = L"";
        mpDoMainList->HomeUp();
        mpDoMainList->SetAllUnSelectedItem();
        mpDoMainList->RemoveAllShow();
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        mpThroughtputTestEdt->SetText(L"");
        OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
        //JGW_DeleteFile(mstrSaveOutPutLog.c_str());
        //! SetTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO,1000,NULL);
        bUpdateLogTime = true;
        return true;
    }

    bool CCTSEThroughTestDlg::OnFailTestStatus()
    {
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        /*
        if (!JGW_DeleteFile(mstrSaveOutPutLog.c_str())) 
        {
            JGW_KillProcessToName(L"adb.exe");
            Sleep(1000);
            JGW_DeleteFile(mstrSaveOutPutLog.c_str());
        }
        */
#ifdef SHOW_TESTPROJECT_INFO
        if (!mstrPrevTestProjectName.empty())
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"%s => FAIL",mstrPrevTestProjectName.c_str());
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
            mstrPrevTestProjectName = L"";
        }
#endif
        if (mbIsSaveTestLog) 
        {
            //bool result = CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",false,mpThroughtputTestEdt->GetText().GetData());
            bool result = CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",false,mstrTestLogStream.str());
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"Save Test Log Result %s",result ? L"true" : L"false");
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
        }
        //! KillTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO);
        return true;
    }

    void CCTSEThroughTestDlg::OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (mpDoMainList)
        {
            mpDoMainList->SetNeedEndDown(true);
        }
    }

    bool CCTSEThroughTestDlg::OnPassTestStatus()
    {
        //! KillTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO);
#ifdef SHOW_TESTPROJECT_INFO
        if (!mstrPrevTestProjectName.empty())
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"%s => PASS",mstrPrevTestProjectName.c_str());
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
            mstrPrevTestProjectName = L"";
        }
#endif
		if (mbIsSaveTestLog)
        {
            //bool result = CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",true,mpThroughtputTestEdt->GetText().GetData());
			bool result = CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",true,mstrTestLogStream.str());
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"Save Test Log Result %s",result ? L"true" : L"false");
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
        }
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_AUTO_TEST,WPARAM(NULL),0);

        return true;
    }

    std::wstring CCTSEThroughTestDlg::GetSingleTestTime(double dTimer)
    {
        std::wstring strTimer;
        if (dTimer < 100)
        {
            FormatWString(strTimer,L"%.2lf ms",dTimer);
        }
        else if (dTimer >= 100 && dTimer < 1000 * 1000)
        {
            FormatWString(strTimer,L"%.2lf s",dTimer/1000.00);
        }
        else
        {
            FormatWString(strTimer,L"%.1lf min",dTimer/(1000.00 * 60.00));
        }
        return strTimer;
    }

    void CCTSEThroughTestDlg::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        _ASSERT(wParam);
        _ASSERT(lParam);
        DWORD nRgn = (WPARAM)lParam;
        LPCTSTR strTestProject = (LPCTSTR)wParam;
        CListTextElementUI* pListTextElementUi = NULL;
        int  nIndex = mpDoMainList->GetCount();
        std::wstring strTemp;

        switch ( nRgn )
        {
        case BALCK:
            FormatWString(strTemp,L"%d",nIndex+1);
            pListTextElementUi = mpDoMainList->InsertItem( mpDoMainList->GetList()->GetCount(),strTemp.c_str(),30);
            mpDoMainList->SetNeedEndDown(true);
            pListTextElementUi->SetText(0,strTemp.c_str());
            pListTextElementUi->SetText(1,strTestProject);
            pListTextElementUi->SetText(2,L"");
            pListTextElementUi->SetText(3,L"---");
            pListTextElementUi->SetText(4,L"---");
            pListTextElementUi->Select(false);
#ifdef SHOW_TESTPROJECT_INFO
            if (nIndex >= 1 && !mstrPrevTestProjectName.empty() && bUpdateLogTime)
            {
                JGW_FormatWString(strTemp,L"%s => PASS",mstrPrevTestProjectName.c_str());
                OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
                mstrPrevTestProjectName = L"";
            }
            if (bUpdateLogTime)
            {
                JGW_FormatWString(strTemp,L"%s => Runing",strTestProject);
                OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
                mstrPrevTestProjectName = strTestProject;
            }
#endif
            m_cHiperTime.Start();
            break;
        case RED:
            pListTextElementUi = mpDoMainList->GetListTextElementItemClass(nIndex - 1);
            if(!pListTextElementUi) return ;
            m_cHiperTime.Stop();    
            //! pListTextElementUi->SetText(1,strTestProject);
            pListTextElementUi->SetText(3,GetSingleTestTime(m_cHiperTime.GetTimeMs()).c_str());
            pListTextElementUi->SetText(4,L"F");
            pListTextElementUi->Select(true);
            break;
        case GREEN:
            pListTextElementUi = mpDoMainList->GetListTextElementItemClass(nIndex-1);
            if(!pListTextElementUi) return ;
            m_cHiperTime.Stop();
            pListTextElementUi->SetText(3,GetSingleTestTime(m_cHiperTime.GetTimeMs()).c_str());
            pListTextElementUi->SetText(4,L"P");
            pListTextElementUi->Select(false);
            break;
        }  
    }

    void CCTSEThroughTestDlg::OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam)
    {
        MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),GetString(E_TSE_INITFAIL_CHECKCONFIG_LOG),L"TSE INTI",MB_ICONERROR);
        PostMessage(msTSEThroughputTestConfigParam.mhMainWnd,WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS,NULL,NULL);
    }

    void CCTSEThroughTestDlg::OnTSEShowTestContent(const WPARAM& wParam,const LPARAM& lParam)
    {
        CListTextElementUI* pListTextElementUi = mpDoMainList->GetListTextElementItemClass(mpDoMainList->GetCount() - 1);
        if(!pListTextElementUi) return ;
        pListTextElementUi->SetText(2,(LPCTSTR)wParam);
    }

    void CCTSEThroughTestDlg::OnAppendRichEditTest(const WPARAM& wParam,const LPARAM& lParam)
    {
        //IsBadStringPtr((const wchar_t*)wParam,-1);
        std::wstring strTemp((const wchar_t*)wParam);
        //std::string log = JGW_W2A(strTemp,CP_UTF8);
        //strTemp = JGW_A2W(log,CP_UTF8);
        JGW_EraseLastAndFristTwoCharsW(strTemp);
        mpThroughtputTestEdt->AppendText(strTemp.c_str());
        mpThroughtputTestEdt->AppendText(L"\r\n");
        mpThroughtputTestEdt->EndDown();

        mstrTestLogStream << strTemp << std::endl;
    }

    void CCTSEThroughTestDlg::OnTimerUpdateLogInfo()
    {
#if 0
        std::wifstream mFileStream;
        std::wstring strTemp;

        if (msTSEThroughputTestConfigParam.mnTestTime > msTSEThroughputTestConfigParam.mnTestTimeIndex)
            JGW_FormatWString(strTemp,L"%d.0-%s%d.0",msTSEThroughputTestConfigParam.mnTestTimeIndex,msTSEThroughputTestConfigParam.mnTestTimeIndex + 1 < 10?L" ":L"",msTSEThroughputTestConfigParam.mnTestTimeIndex + 1);
        else 
        {
            JGW_FormatWString(strTemp,L"0.0-%d.0",msTSEThroughputTestConfigParam.mnTestTimeIndex);
            //KillTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO);
            bUpdateLogTime = true;
        }

        mFileStream.open(mstrSaveOutPutLog.c_str());
        if (!mFileStream.is_open()) return ;
        mFileStream.seekg(msTSEThroughputTestConfigParam.mnSeekgLog);

        while (mFileStream.getline(szReadBuf,READ_BUFFER_MAX_LEN))
        {
            if (NULL != wcsstr(szReadBuf,strTemp.c_str()))
            {
                if (msTSEThroughputTestConfigParam.mbMultThread && !wcsstr(szReadBuf,L"[SUM]")) continue;
                msTSEThroughputTestConfigParam.mnSeekgLog = mFileStream.tellg();
                OnAppendRichEditTest(WPARAM(szReadBuf),NULL);

                //! update 
//                 if (msTSEThroughputTestConfigParam.mnTestTime > msTSEThroughputTestConfigParam.mnTestTimeIndex ++)
//                 {
//                     std::vector<std::wstring> vstrTemp;
//                     JGW_ParserStrW(szReadBuf,L" ",vstrTemp);

                    /*if (msTSEThroughputTestConfigParam.mbMultThread)
                        mpView->mpWkeBrowser->UpdateBpsInfo(msTSEThroughputTestConfigParam.mnTestTimeIndex,_ttof(vstrTemp[5].c_str()));
                    else 
                        mpView->mpWkeBrowser->UpdateBpsInfo(msTSEThroughputTestConfigParam.mnTestTimeIndex,_ttof(vstrTemp[6].c_str()));                 
                    break;*/
/*                } */
            } 
            else
            {
                msTSEThroughputTestConfigParam.mnSeekgLog = mFileStream.tellg();
                OnAppendRichEditTest(WPARAM(szReadBuf),NULL);
            }
        }
        mFileStream.close();
#endif
    }
    /// JSON数据
    void CCTSEThroughTestDlg::OnTSEAdapterLog(const WPARAM& wParam,const LPARAM& lParam)
    {
        COPYDATASTRUCT* pCopyDateStruct = (COPYDATASTRUCT*)lParam;
        if (0 == pCopyDateStruct->cbData)
        {
            return;
        }
#if 1
        ((CCTSEThroughputTestThread*)mpTestThread)->UpdateCopyDataMessage((char*)pCopyDateStruct->lpData);
#else
        CCJGW_FTM_Json ftmJson;
        if (ftmJson.FromJsonToString((char*)pCopyDateStruct->lpData))
        {
            CCJGW_CopyDataTestLogJson testLog;
            testLog.FromJosn(ftmJson);
            switch ((CopyDataLogTypePluginDomain)testLog.LogType)
            {
            case Debug_COPYDATA:
            case Info_COPYDATA:
                PDLOG4WW(testLog.Message.c_str());
                break;
            case Warn_COPYDATA:
            case Error_COPYDATA:
                OnAppendRichEditTest(WPARAM(testLog.Message.c_str()),NULL);
                PELOG4WW(testLog.Message.c_str());
                break;
            case QIAWindowHanlde_COPYDATA:
                ((CCTSEThroughputTestThread*)mpTestThread)->UpdateQIAWindowHandle(_ttoi(testLog.Message.c_str()));
                break;
            case CheckQIAIsRunning_COPYDATA:
                ((CCTSEThroughputTestThread*)mpTestThread)->UpdateCheckQIAIsRunningFlag(1);
                break;
            case QIAXttTreeTestComplete_COPYDATA:
                ((CCTSEThroughputTestThread*)mpTestThread)->UpdateQiaTestRessult(1 == _ttoi(testLog.Message.c_str()));
                break;
            case UserDefine_COPYDATA:
                switch ((CopyDataUserDefineIDType)testLog.UserDefineID)
                {
                case E_COPYDATA_USER_DEFINE_HTMLFILEPATH:
                    PHLOG4WW(testLog.Message.c_str());
                    OnAppendRichEditTest(WPARAM(testLog.Message.c_str()),NULL);
                    ((CCTSEThroughputTestThread*)mpTestThread)->AddHtmlTestLogging(testLog.Message);
                    break;
                case E_COPYDATA_TSE_SHOW_CONTENT_INFO:
                    OnTSEShowTestContent(WPARAM(testLog.Message.c_str()),NULL);
                    break;
                case E_COPYDATA_TSE_SHOW_TEST_INFO_TO_RICHEDIT:
                    OnAppendRichEditTest(WPARAM(testLog.Message.c_str()),NULL);
                    break;
                case E_COPYDATA_TSE_SHOW_IDEL_BUSY_LIST_TEST_STATUS:
                    OnTestListMsg(WPARAM(testLog.Message.c_str()),BALCK);
                    OnTestListMsg(WPARAM(testLog.Message.c_str()),BULE);
                    break;
                case E_COPYDATA_TSE_SHOW_FAIL_LIST_TEST_STATUS:
                    OnTestListMsg(WPARAM(testLog.Message.c_str()),RED);
                    break;
                case E_COPYDATA_TSE_SHOW_PASS_LIST_TEST_STATUS:
                    OnTestListMsg(WPARAM(testLog.Message.c_str()),GREEN);
                    break;
                }

                break;
            }

        }
#endif
    }
}