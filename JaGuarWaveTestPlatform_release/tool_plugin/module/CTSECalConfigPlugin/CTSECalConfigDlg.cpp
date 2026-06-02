#include "StdAfx.h"
#include "CTSECalConfigDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
#include "CTSECalConfigTestThread.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include "CTSEGuCalDMG.h"
#include "CTSEGuCalWiFiIPQ40xx.h"
#include "CTSEGuCalRF.h"
#include "CTSEGuCalWlanLoss.h"
#include <fstream>
#include <JGW_TestLoggingPlugin/CJGW_SaveTestLog.h>


namespace JGW
{
    CCTSECalConfigDlg::CCTSECalConfigDlg(void) : mpTestThread(NULL),mpTSEGuCalBase(NULL),mbIsRunTest(false)
    {
        msPluginSkin.mstrSkinXmlPath = L"calibration_configuration_plugin_ui.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }


    CCTSECalConfigDlg::~CCTSECalConfigDlg(void)
    {
    }

    void CCTSECalConfigDlg::CloseSubWndPlugin()
    {
        KillTimer(msTSECalConfigTestConfig.mhMainWnd,WM_TIMER_UPDATE_TEST_TIME);
        if (IsCloseSubWnd()) return;
        if (mpTestThread)
        {
            mpTestThread->TerminateMessageThread();
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
    }

    void CCTSECalConfigDlg::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
    }

    bool CCTSECalConfigDlg::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
    {
        //! init ui
        {
            mpViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
            mpPluginPaintManagerUI = (CPaintManagerUI*)mpViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
            msTSECalConfigTestConfig.mhMainWnd = mpPluginPaintManagerUI->GetPaintWindow();
            if ( !GetDuiContronlClass() || !InitGuCalConfig()) return false;

            GetUIPtr(CalConfigHorizontalLayout)->SetBkColor(IDLE_BK_COLOR);
            GetUIPtr(CalTestStatusBtn)->SetTextColor(IDLE_TEXT_COLOR);
            GetUIPtr(CalTestStatusBtn)->SetBkColor(IDLE_BK_COLOR);
            GetUIPtr(CalTestStatusBtn)->SetText(L"IDLE");  
            GetUIPtr(CalConfigDomainlist)->SetAttribute(L"itemalign",L"left");
            GetUIPtr(CalConfigDomainlist)->GetList()->RemoveAll();
            GetUIPtr(CalConfigDomainlist)->GetHeader()->SetScaleHeader(false);
            GetUIPtr(CalConfigDomainlist)->GetHeader()->RemoveAll();

            GetUIPtr(CalConfigDomainlist)->InsertColumn(0,_T("Index"),60);
            GetUIPtr(CalConfigDomainlist)->InsertColumn(1,_T("Test Project"),0);
            GetUIPtr(CalConfigDomainlist)->InsertColumn(2,_T("Content"),200);
            GetUIPtr(CalConfigDomainlist)->InsertColumn(3,_T("Time"),80);
            GetUIPtr(CalConfigDomainlist)->InsertColumn(4,_T("P/F"),40);

            
            msTSECalConfigTestConfig.mbInitSequenceTestOk = false;
            msTSECalConfigTestConfig.mstrSuiteXmlConfigPath = JGW_GetTSEConfigFilePath(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpViewResolverParam->mpPluginInfo->mstrSuiteXmlName);
        }
        //! init ui msg
        {
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_TIMER,&CCTSECalConfigDlg::OnRspTimeMsg);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TSE_SHOW_CONTENT,&CCTSECalConfigDlg::OnTSEShowTestContent);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_LIST_MSG,&CCTSECalConfigDlg::OnTestListMsg);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_INIT_TSE_TEST_FAIL_MSG,&CCTSECalConfigDlg::OnTSEInitFail);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_RICHEDIT_APPEND_TEST_MSG,&CCTSECalConfigDlg::OnAppendRichEditTest);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_TEST_FAIL_MSG,&CCTSECalConfigDlg::OnTestFailMsg);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_TEST_PASS_MSG,&CCTSECalConfigDlg::OnTestPassMsg);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_SAVE_GU_CAL_DATA,&CCTSECalConfigDlg::OnRspSaveGuCalDataResult);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_COPYDATA,&CCTSECalConfigDlg::OnTSEAdapterLog);
        }
        
        //! Init Test Thread
        {
            if (!mpTestThread) mpTestThread = new CCTSECalConfigTestThread;
            if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

            return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&msTSECalConfigTestConfig));
        }   

        return true;
    }

    void CCTSECalConfigDlg::OnTSEAdapterLog(const WPARAM& wParam,const LPARAM& lParam)
    {
        COPYDATASTRUCT* pCopyDateStruct = (COPYDATASTRUCT*)lParam;
        if (0 == pCopyDateStruct->cbData)
        {
            return;
        }
#if 1
        ((CCTSECalConfigTestThread*)mpTestThread)->UpdateCopyDataMessage((char*)pCopyDateStruct->lpData);
#endif
    }

    void CCTSECalConfigDlg::OnRspTimeMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (WM_TIMER_UPDATE_TEST_TIME == (int)wParam)
        {
            std::wstring sztime(_T(""));
            mdwTestTime++;
            if (mdwTestTime > 60 * 60) mdwTestTime = 0;
            JGW::FormatWString(sztime,L"%02d:%02d",mdwTestTime / 60,mdwTestTime % 60);
            GetUIPtr(CalConfigTestTimeBtn)->SetText(sztime.c_str());
        }
    }

    void CCTSECalConfigDlg::OnTSEShowTestContent(const WPARAM& wParam,const LPARAM& lParam)
    {
        CListTextElementUI* pListTextElementUi = GetUIPtr(CalConfigDomainlist)->GetListTextElementItemClass(GetUIPtr(CalConfigDomainlist)->GetCount() - 1);
        if(!pListTextElementUi) return ;
        pListTextElementUi->SetText(2,(LPCTSTR)wParam);
    }

    void CCTSECalConfigDlg::OnAppendRichEditTest(const WPARAM& wParam,const LPARAM& lParam)
    {
        std::wstring strTemp((const wchar_t*)wParam);
        JGW_EraseLastAndFristTwoCharsW(strTemp);
        GetUIPtr(CalConfigTestLogEdt)->AppendText(strTemp.c_str());
        GetUIPtr(CalConfigTestLogEdt)->AppendText(L"\r\n");
        GetUIPtr(CalConfigTestLogEdt)->EndDown();

        mstrTestLogStream << strTemp << std::endl;
    }

    void CCTSECalConfigDlg::OnNotifySubWndPlugin(LPVOID pMsg)
    {
        TNotifyUI* pNotifyMsg = (TNotifyUI*)pMsg;
        if (pNotifyMsg->sType == DUI_MSGTYPE_CLICK)
        {
            if (pNotifyMsg->pSender == GetUIPtr(CalConfigStartBtn))
            {
				if(msTSECalConfigTestConfig.meTSECalType == E_TSE_CAL_RF && msTSECalConfigTestConfig.mvsGuData.empty())
				{
					GetUIPtr(CalConfigStartBtn)->SetEnabled(true);
					MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),L"请导入金机数据文件",L"Error",MB_ICONERROR);
					return ;
				}
                GetUIPtr(CalConfigStartBtn)->SetEnabled(false);
                OnStartCalClickMsg();
                SetTimer(msTSECalConfigTestConfig.mhMainWnd,WM_TIMER_UPDATE_TEST_TIME,1000,NULL);
            }
            else if (pNotifyMsg->pSender == GetUIPtr(CalConfigSaveBtn))
            {
                GetUIPtr(CalConfigSaveBtn)->SetEnabled(false);
                OnSaveCalClickMsg();
            }
            else if (pNotifyMsg->pSender == GetUIPtr(ExportGuConfigConfigBtn))
            {
                OnImportClickMsg();
            }
            else if (pNotifyMsg->pSender == GetUIPtr(CalConfigConfigBtn))
            {
                if (!mbIsRunTest) mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
            }
        }
    }

    void CCTSECalConfigDlg::OnStartCalClickMsg()
    {
        mdwTestTime = 0;
        mbIsRunTest = true;
        mstrTestLogStream.str(L"");
        mstrTestLogStream.clear();

        GetUIPtr(CalTestStatusBtn)->SetTextColor(BUSY_TEXT_COLOR);
        GetUIPtr(CalTestStatusBtn)->SetBkColor(BUSY_BK_COLOR);
        GetUIPtr(CalConfigHorizontalLayout)->SetBkColor(BUSY_BK_COLOR);
        GetUIPtr(CalTestStatusBtn)->SetText(L"BUSY");
        GetUIPtr(CalConfigTestTimeBtn)->SetText(L"00:00");

        GetUIPtr(CalConfigTestLogEdt)->SetText(L"");
        GetUIPtr(CalConfigDomainlist)->HomeUp();
        GetUIPtr(CalConfigDomainlist)->SetAllUnSelectedItem();
        GetUIPtr(CalConfigDomainlist)->RemoveAllShow();
        GetUIPtr(CalConfigDomainlist)->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        
		mpTSEGuCalBase->OnStartDutTestThread();
        //! mpsTSETThroughputTestConfigParam->mfGuStartLoss = mpSequenceManageImpl->GetGlobalEnvironmentImpl()->GetFloat(L"TSE_GU_LOSS");
        if (E_TSE_QIA_WLAN == msTSECalConfigTestConfig.meTSECalType)
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"Is Save Test Log : %s LF\r...",true ? L"true" : L"false");
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
        }
        
        PostMessage(mpViewResolverParam->mpMainPaintManager->GetPaintWindow(),WM_HWND_VIEW_LOG_INFO,WPARAM(0),NULL);
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
    }

    void CCTSECalConfigDlg::OnTestFailMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mbIsRunTest = false;
        GetUIPtr(CalConfigStartBtn)->SetEnabled(true);
        GetUIPtr(CalTestStatusBtn)->SetTextColor(FAIL_TEXT_COLOR);
        GetUIPtr(CalTestStatusBtn)->SetBkColor(FAIL_BK_COLOR);
        GetUIPtr(CalConfigHorizontalLayout)->SetBkColor(FAIL_BK_COLOR);
        GetUIPtr(CalTestStatusBtn)->SetText(L"FAIL");
        KillTimer(msTSECalConfigTestConfig.mhMainWnd,WM_TIMER_UPDATE_TEST_TIME);

        if (E_TSE_QIA_WLAN == msTSECalConfigTestConfig.meTSECalType) 
        {
            bool result = CCJGW_SaveTestLog::SaveTestLog(mpTSEGuCalBase->GetDutTestDataFilePath(),L"",false,mstrTestLogStream.str());
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"Save Test Log Result %s",result ? L"true" : L"false");
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
        }
        PostMessage(mpViewResolverParam->mpMainPaintManager->GetPaintWindow(),WM_HWND_TEST_FAIL_MSG,WPARAM(0),NULL);
    }

    void CCTSECalConfigDlg::OnTestPassMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mbIsRunTest = false;
        GetUIPtr(CalConfigStartBtn)->SetEnabled(true);
        GetUIPtr(CalTestStatusBtn)->SetTextColor(PASS_TEXT_COLOR);
        GetUIPtr(CalTestStatusBtn)->SetBkColor(PASS_BK_COLOR);
        GetUIPtr(CalConfigHorizontalLayout)->SetBkColor(PASS_BK_COLOR);
        GetUIPtr(CalTestStatusBtn)->SetText(L"PASS");
        KillTimer(msTSECalConfigTestConfig.mhMainWnd,WM_TIMER_UPDATE_TEST_TIME);

        if (E_TSE_QIA_WLAN == msTSECalConfigTestConfig.meTSECalType) 
        {
            bool result = CCJGW_SaveTestLog::SaveTestLog(mpTSEGuCalBase->GetDutTestDataFilePath(),L"",true,mstrTestLogStream.str());
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"Save Test Log Result %s",result ? L"true" : L"false");
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
        }
        PostMessage(mpViewResolverParam->mpMainPaintManager->GetPaintWindow(),WM_HWND_TEST_PASS_MSG,WPARAM(0),NULL);
    }

    void CCTSECalConfigDlg::OnSaveCalClickMsg()
    {
        mpTestThread->PostThreadMessage(WM_THREAD_SAVE_GU_CAL_DATA,WPARAM(mpTSEGuCalBase),0);
        //PostMessage(mpViewResolverParam->mpMainPaintManager->GetPaintWindow(),WM_THREAD_SAVE_GU_CAL_DATA,WPARAM(mpTSEGuCalBase),NULL);
    }

    void CCTSECalConfigDlg::OnRspSaveGuCalDataResult(const WPARAM& wParam,const LPARAM& lParam)
    {
        GetUIPtr(CalConfigSaveBtn)->SetEnabled(true);
        bool result = wParam == 1 ? true : false;
        if (result)
        {
            MessageBox(msTSECalConfigTestConfig.mhMainWnd,L"保存校准金机数据成功",L"OK",MB_OK);
        }
        else
        {
            MessageBox(msTSECalConfigTestConfig.mhMainWnd,L"保存校准金机数据失败",L"Fail",MB_OK);
        }
    }

    bool CCTSECalConfigDlg::GetDuiContronlClass()
    {
        bool bSuccess = true;

        FindUIPtr(CalConfigTestTimeBtn,CButtonUI);
        FindUIPtr(CalConfigStartBtn,CButtonUI);
        FindUIPtr(CalConfigSaveBtn,CButtonUI);
        FindUIPtr(CalConfigConfigBtn,CButtonUI);
        FindUIPtr(CalConfigDomainlist,CListTestCtrlUI);
        FindUIPtr(CalConfigTestLogEdt,CRichEditUI);
        FindUIPtr(CalConfigHorizontalLayout,CHorizontalLayoutUI);
        FindUIPtr(CalTestStatusBtn,CButtonUI);
        FindUIPtr(ExportGuConfigConfigBtn,CButtonUI);

        bSuccess &= (NULL != GetUIPtr(CalConfigTestTimeBtn));
        bSuccess &= (NULL != GetUIPtr(CalConfigStartBtn));
        bSuccess &= (NULL != GetUIPtr(CalConfigSaveBtn));
        bSuccess &= (NULL != GetUIPtr(CalConfigConfigBtn));
        bSuccess &= (NULL != GetUIPtr(CalConfigDomainlist));
        bSuccess &= (NULL != GetUIPtr(CalConfigTestLogEdt));
        bSuccess &= (NULL != GetUIPtr(CalConfigHorizontalLayout));
        bSuccess &= (NULL != GetUIPtr(CalTestStatusBtn));
        bSuccess &= (NULL != GetUIPtr(ExportGuConfigConfigBtn));
        if( !bSuccess ) ::MessageBox( mpPluginPaintManagerUI->GetPaintWindow(),L"Lost skin control",L"error",MB_ICONERROR);

        return bSuccess;
    }

    void CCTSECalConfigDlg::OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam)
    {
        MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),GetString(E_TSE_INITFAIL_CHECKCONFIG_LOG),L"TSE INTI",MB_ICONERROR);
        PostMessage(msTSECalConfigTestConfig.mhMainWnd,WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS,NULL,NULL);
    }


    std::wstring CCTSECalConfigDlg::GetSingleTestTime(double dTimer)
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

    void CCTSECalConfigDlg::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        _ASSERT(wParam);
        _ASSERT(lParam);
        DWORD nRgn = (WPARAM)lParam;
        LPCTSTR strTestProject = (LPCTSTR)wParam;
        CListTextElementUI* pListTextElementUi = NULL;
        int  nIndex = GetUIPtr(CalConfigDomainlist)->GetCount();
        std::wstring strTemp;

        switch ( nRgn )
        {
        case BALCK:
            FormatWString(strTemp,L"%d",nIndex+1);
            pListTextElementUi = GetUIPtr(CalConfigDomainlist)->InsertItem( GetUIPtr(CalConfigDomainlist)->GetList()->GetCount(),strTemp.c_str(),30);
            GetUIPtr(CalConfigDomainlist)->SetNeedEndDown(true);
            pListTextElementUi->SetText(0,strTemp.c_str());
            pListTextElementUi->SetText(1,strTestProject);
            pListTextElementUi->SetText(2,L"");
            pListTextElementUi->SetText(3,L"---");
            pListTextElementUi->SetText(4,L"---");
            pListTextElementUi->Select(false);
            m_cHiperTime.Start();
            break;
        case RED:
            pListTextElementUi = GetUIPtr(CalConfigDomainlist)->GetListTextElementItemClass(nIndex - 1);
            if(!pListTextElementUi) return ;
            m_cHiperTime.Stop();    
            //! pListTextElementUi->SetText(1,strTestProject);
            pListTextElementUi->SetText(3,GetSingleTestTime(m_cHiperTime.GetTimeMs()).c_str());
            pListTextElementUi->SetText(4,L"F");
            pListTextElementUi->Select(true);
            break;
        case GREEN:
            pListTextElementUi = GetUIPtr(CalConfigDomainlist)->GetListTextElementItemClass(nIndex-1);
            if(!pListTextElementUi) return ;
            m_cHiperTime.Stop();
            pListTextElementUi->SetText(3,GetSingleTestTime(m_cHiperTime.GetTimeMs()).c_str());
            pListTextElementUi->SetText(4,L"P");
            pListTextElementUi->Select(false);
            break;
        }  
    }

    void CCTSECalConfigDlg::OnImportClickMsg()
    {
        std::wstring strGuDataFilePath;
        if( !JGW_ChooseFile(strGuDataFilePath,L"*.csv;*.txt\0*.csv;*.txt\0all file(*.*)\0*.*\0\0",msTSECalConfigTestConfig.mhMainWnd ) )  return ;
        msTSECalConfigTestConfig.mvsGuData.clear();

        std::wifstream inputFile;
        inputFile.imbue( std::locale("") );
        inputFile.open(strGuDataFilePath);
        const int LINE_LENGTH = 1024 * 4; 
        wchar_t str[LINE_LENGTH] = {0};

        while (inputFile.getline(str,LINE_LENGTH))
        {
            std::vector<std::wstring> vTemp;
            JGW_ParserStrW(str,L",",vTemp);
            if (vTemp.size() == 4 && NULL == JGW_WStrCaseWStr(vTemp[0].c_str(),L"Barcode"))
            {
                S_GU_DATA sGuData = {_ttoi(vTemp[1].c_str()),static_cast<float>(_ttof(vTemp[3].c_str())),vTemp[0],vTemp[2]};
                msTSECalConfigTestConfig.mvsGuData.push_back(sGuData);
            }
        }
    }

    bool CCTSECalConfigDlg::InitGuCalConfig()
    {
        std::wstring strGuCalConfigPath = JGW_GetCacheConfigFilePath(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName,L"GuCalConfig.ini");
        if (!JGW_FileExistsToFilePath(strGuCalConfigPath.c_str()))
        {
            MessageBox(msTSECalConfigTestConfig.mhMainWnd,strGuCalConfigPath.c_str(),GetString(E_TSE_FILE_NOT_EXIST),MB_ICONERROR);
            return false;
        }
        CCJGW_ConfigIni configIni;
        configIni.InitIniFilePath(strGuCalConfigPath.c_str());
        msTSECalConfigTestConfig.meTSECalType = (E_TSE_CAL_TYPE)configIni.GetIniKeyIntValue(L"MAIN",L"TSEGuCalType");
        if (mpTSEGuCalBase)
        {
            delete mpTSEGuCalBase;
            mpTSEGuCalBase = NULL;
        }

        switch (msTSECalConfigTestConfig.meTSECalType)
        {
        case E_TSE_CAL_DMG:
            mpTSEGuCalBase = new CCTSEGuCalDMG;
            break;
        case E_TSE_CAL_IPQ40XXWIFI:
            mpTSEGuCalBase = new CCTSEGuCalWiFiIPQ40xx;
            break;
        case E_TSE_CAL_RF:
            mpTSEGuCalBase = new CCTSEGuCalRF;
            break;
        case E_TSE_QIA_WLAN:
            mpTSEGuCalBase = new CCTSEGuCalWlanLoss;
            break;
        default:
            mpTSEGuCalBase = new CCTSEGuCalDMG;
        }

        if(!mpTSEGuCalBase->LoadGuCalConfig(strGuCalConfigPath,&msTSECalConfigTestConfig))
        {
            MessageBox(msTSECalConfigTestConfig.mhMainWnd,strGuCalConfigPath.c_str(),L"Load Gu Cal Config Fail",MB_ICONERROR);
            return false;
        }

        return true;
    }
}

