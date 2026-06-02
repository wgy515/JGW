#include "StdAfx.h"
#include "CJGW_AutomationPluginDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CCJGW_AutomationPluginDlg::CCJGW_AutomationPluginDlg(void) : mpTestThread(NULL),m_nListItemHeight(30),mpDoMainList(NULL),mpAsyncWndMessage(NULL),mnTestCountIndex(0)
    {
        msPluginSkin.mstrSkinXmlPath = L"test.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
        msTSETestConfigParam.mbInitSequenceTestOk = false;
        msTSETestConfigParam.mbReTest = true;
        msTSETestConfigParam.mnTestCount = -1;
        mbIsShowLogHwnd = false;
        mbIsShowTestResultStatus = false;
    }


    CCJGW_AutomationPluginDlg::~CCJGW_AutomationPluginDlg(void)
    {
    }

    void CCJGW_AutomationPluginDlg::CloseSubWndPlugin()
    {
        if (mpTestThread)
        {
            msTSETestConfigParam.mbReTest = false;
            mpTestThread->TerminateMessageThread();
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    bool CCJGW_AutomationPluginDlg::OnIdelTestStatus()
    {
        mpStartTestBtn = static_cast<CButtonUI*>(mpViewResolverParam->mpMainPaintManager->FindControl(L"startBtn"));
        mpDoMainList = static_cast<CListTestCtrlUI*>(mpPluginPaintManagerUI->FindControl(L"domainlist"));
        mpReTest = static_cast<CCheckBoxUI*>(mpViewResolverParam->mpMainPaintManager->FindControl(L"ReTest"));
        if (!mpDoMainList || !mpReTest || !mpStartTestBtn) return false;

        //! INIT UI 
        {
            mpReTest->SetVisible(true);
            mpReTest->SetCheck(true);
#if 0
            mpDoMainList->SetAttribute(L"itemalign",L"center");
            mpDoMainList->GetList()->RemoveAll();
            mpDoMainList->GetHeader()->SetScaleHeader(true);

            mpDoMainList->GetHeader()->GetItemAt( 0 )->SetText(_T("Index"));
            mpDoMainList->GetHeader()->GetItemAt( 1 )->SetText(_T("Test Project"));
            mpDoMainList->GetHeader()->GetItemAt( 2 )->SetText(_T("Time"));
            mpDoMainList->InsertColumn(3,_T("P/F"),0);

            mpDoMainList->GetHeader()->GetItemAt( 0 )->SetScale( 10 );
            mpDoMainList->GetHeader()->GetItemAt( 1 )->SetScale( 70 );
            mpDoMainList->GetHeader()->GetItemAt( 2 )->SetScale( 10 );
            mpDoMainList->GetHeader()->GetItemAt( 3 )->SetScale( 10 );
#else
            mpDoMainList->SetAttribute(L"itemalign",L"left");
            mpDoMainList->GetList()->RemoveAll();
            mpDoMainList->GetHeader()->SetScaleHeader(false);
            mpDoMainList->GetHeader()->RemoveAll();

            mpDoMainList->InsertColumn(0,_T("Index"),60);
            mpDoMainList->InsertColumn(1,_T("Test Project"),0);
            mpDoMainList->InsertColumn(2,_T("Content"),0);
            mpDoMainList->InsertColumn(3,_T("Time"),80);
            mpDoMainList->InsertColumn(4,_T("P/F"),40);

            /*mpDoMainList->GetHeader()->GetItemAt(1)->SetAutoCalcWidth(true);*/
#endif
        }
        //! Init Param
        {
            msTSETestConfigParam.mhMainWnd = mpViewResolverParam->mpMainPaintManager->GetPaintWindow();
            msTSETestConfigParam.mstrSuiteXmlConfigPath = JGW_GetTSEConfigFilePath(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpViewResolverParam->mpPluginInfo->mstrSuiteXmlName);
            /*msTSETestConfigParam.mstrSuiteXmlConfigPath = JGW_GetApplicationFolder();
            msTSETestConfigParam.mstrSuiteXmlConfigPath += L"cache\\";
            msTSETestConfigParam.mstrSuiteXmlConfigPath += mpViewResolverParam->mpPluginInfo->mstrModuleFolderName;
            msTSETestConfigParam.mstrSuiteXmlConfigPath += L"\\suite.xml";*/
        }

        {
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TSE_SHOW_CONTENT,&CCJGW_AutomationPluginDlg::OnTSEShowTestContent);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_LIST_MSG,&CCJGW_AutomationPluginDlg::OnTestListMsg);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_INIT_TSE_TEST_FAIL_MSG,&CCJGW_AutomationPluginDlg::OnTSEInitFail);
        }
        //! Init Test Thread
        {
            mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class;
            if (!mpTestThread) mpTestThread = new CCJGW_AutomationPluginThread;
            if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

            return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&msTSETestConfigParam));
        }   
    }

    void CCJGW_AutomationPluginDlg::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
    }

    void CCJGW_AutomationPluginDlg::OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam)
    {
        MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),GetString(E_TSE_INITFAIL_CHECKCONFIG_LOG),L"TSE INTI",MB_ICONERROR);
        PostMessage(msTSETestConfigParam.mhMainWnd,WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS,NULL,NULL);
    }

    void CCJGW_AutomationPluginDlg::OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (mpDoMainList)
        {
            mpDoMainList->SetNeedEndDown(true);
        }
    }

    bool CCJGW_AutomationPluginDlg::OnBusyTestStatus()
    {
        mpDoMainList->SetFocus();
        mpDoMainList->HomeUp();
        mpDoMainList->SetAllUnSelectedItem();
        mpDoMainList->RemoveAllShow();
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        return true;
    }

    bool CCJGW_AutomationPluginDlg::OnFailTestStatus()
    {
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        mpStartTestBtn->SetText(L"Start");
        msTSETestConfigParam.mbReTest = mpReTest->GetCheck();
        //! 不进行重测则直接返回
        if (!msTSETestConfigParam.mbReTest) return true;
        //! 测试加减
        mnTestCountIndex ++;
        //! 停止重新测试
        if (mnTestCountIndex == msTSETestConfigParam.mnTestCount) return true;
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);
        return true;
    }

    bool CCJGW_AutomationPluginDlg::OnPassTestStatus()
    {
        mpStartTestBtn->SetText(L"Start");
        msTSETestConfigParam.mbReTest = mpReTest->GetCheck();
        if (!msTSETestConfigParam.mbReTest) goto PASS_TEST;

        mnTestCountIndex ++;
        //! 停止重新测试
        if (mnTestCountIndex == msTSETestConfigParam.mnTestCount) return true;
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TEST_SATRT_MSG);
        return true;
PASS_TEST:
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_AUTO_TEST,WPARAM(NULL),0);
        return true;
    }

    std::wstring CCJGW_AutomationPluginDlg::GetSingleTestTime(double dTimer)
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

    void CCJGW_AutomationPluginDlg::OnTSEShowTestContent(const WPARAM& wParam,const LPARAM& lParam)
    {
        CListTextElementUI* pListTextElementUi = mpDoMainList->GetListTextElementItemClass(mpDoMainList->GetCount() - 1);
        if(!pListTextElementUi) return ;
        pListTextElementUi->SetText(2,(LPCTSTR)wParam);
    }

    void CCJGW_AutomationPluginDlg::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
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
}