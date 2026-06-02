#include "StdAfx.h"
#include "CSTSETestPluginDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CSTSETestThread.h"
#include <fstream>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
namespace JGW
{
    CCSTSETestPluginDlg::CCSTSETestPluginDlg(void) : mpTestThread(NULL)
    {
        msPluginSkin.mstrSkinXmlPath = L"tse_through_plugin_skin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }

    CCSTSETestPluginDlg::~CCSTSETestPluginDlg(void)
    {
    }

    void CCSTSETestPluginDlg::CloseSubWndPlugin()
    {
        if (mpTestThread)
        {
            mpTestThread->TerminateMessageThread();
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    void CCSTSETestPluginDlg::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
    }

    bool CCSTSETestPluginDlg::OnIdelTestStatus()
    {
        mpDoMainList = static_cast<CListTestCtrlUI*>(mpPluginPaintManagerUI->FindControl(L"domainlist"));
        mpThroughtputTestEdt = static_cast<CRichEditUI*>(mpPluginPaintManagerUI->FindControl(L"throughtputTestEdt"));
        if (!mpDoMainList) return false;
        {
            mpDoMainList->SetAttribute(L"itemalign",L"left");
            mpDoMainList->GetList()->RemoveAll();
            mpDoMainList->GetHeader()->SetScaleHeader(false);
            mpDoMainList->GetHeader()->RemoveAll();

            mpDoMainList->InsertColumn(0,_T("索引"),60);
            mpDoMainList->InsertColumn(1,_T("测试项目"),0);
            mpDoMainList->InsertColumn(2,_T("内容"),200);
            mpDoMainList->InsertColumn(3,_T("时间"),80);
            mpDoMainList->InsertColumn(4,_T("P/F"),40);
        }
        //! Init Param
        {
            //msTSEThroughputTestConfigParam.mnTestTime = 30;
            //msTSEThroughputTestConfigParam.mnTestTimeIndex = 0;
            //msTSEThroughputTestConfigParam.mbMultThread = false; //! 是否是多路TCP测试
            //msTSEThroughputTestConfigParam.mhMainWnd = mpPluginPaintManagerUI->GetPaintWindow();
            msCSTSETestConfig.mstrCSTSEClientExecPath = JGW_RealativePathToAbsPath(L"CSTSEClientTool.exe");
            msCSTSETestConfig.mstrSuiteXmlConfigPath = JGW_GetTSEConfigFilePath(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName);
            msCSTSETestConfig.mmapExecParam[TSE_SUITE_FILE_PATH] = msCSTSETestConfig.mstrSuiteXmlConfigPath;
            msCSTSETestConfig.mmapExecParam[TSE_GLOBAL_MAIN_HWND] =  JGW_GetFormatWString(L"%d",mpPluginPaintManagerUI->GetPaintWindow());
        }

        {
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TSE_SHOW_CONTENT,&CCSTSETestPluginDlg::OnTSEShowTestContent);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_LIST_MSG,&CCSTSETestPluginDlg::OnTestListMsg);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_INIT_TSE_TEST_FAIL_MSG,&CCSTSETestPluginDlg::OnTSEInitFail);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_RICHEDIT_APPEND_TEST_MSG,&CCSTSETestPluginDlg::OnAppendRichEditTest);
            
        }
        //! Init Test Thread
        {
            if (!mpTestThread) mpTestThread = new CCSTSETestThread;
            if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

            return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&msCSTSETestConfig));
        }   

        return true;
    }


    bool CCSTSETestPluginDlg::OnBusyTestStatus()
    {
        mpDoMainList->HomeUp();
        mpDoMainList->SetAllUnSelectedItem();
        mpDoMainList->RemoveAllShow();
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        mpThroughtputTestEdt->SetText(L"");
        return true;
    }

    bool CCSTSETestPluginDlg::OnFailTestStatus()
    {
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        return true;
    }

    bool CCSTSETestPluginDlg::OnPassTestStatus()
    {
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_AUTO_TEST,WPARAM(NULL),0);
        return true;
    }

    std::wstring CCSTSETestPluginDlg::GetSingleTestTime(double dTimer)
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

    void CCSTSETestPluginDlg::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
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

    void CCSTSETestPluginDlg::OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam)
    {
        MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),L"初始化配置失败，请检查配置是否正常,详情见Log",L"TSE INTI",MB_ICONERROR);
        PostMessage(mpPluginPaintManagerUI->GetPaintWindow(),WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS,NULL,NULL);
    }

    void CCSTSETestPluginDlg::OnTSEShowTestContent(const WPARAM& wParam,const LPARAM& lParam)
    {
        CListTextElementUI* pListTextElementUi = mpDoMainList->GetListTextElementItemClass(mpDoMainList->GetCount() - 1);
        if(!pListTextElementUi) return ;
        pListTextElementUi->SetText(2,(LPCTSTR)wParam);
    }

    void CCSTSETestPluginDlg::OnAppendRichEditTest(const WPARAM& wParam,const LPARAM& lParam)
    {
        std::wstring strTemp((const wchar_t*)wParam);
        JGW_EraseLastAndFristTwoCharsW(strTemp);
        mpThroughtputTestEdt->AppendText(strTemp.c_str());
        mpThroughtputTestEdt->AppendText(L"\r\n");
        mpThroughtputTestEdt->EndDown();
    }
}