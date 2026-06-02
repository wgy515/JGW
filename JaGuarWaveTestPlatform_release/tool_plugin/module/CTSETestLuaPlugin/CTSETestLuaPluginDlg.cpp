#include "StdAfx.h"
#include "CTSETestLuaPluginDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CTSEThroughputTestThread.h"
#include <JGW_TestLoggingPlugin/CJGW_SaveTestLog.h>
#include <fstream>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>

namespace JGW
{
    CCTSETestLuaPluginDlg::CCTSETestLuaPluginDlg(void)
    {
        msPluginSkin.mstrSkinXmlPath = L"tse_through_plugin_skin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
        mbIsShowLogHwnd = false;
    }


    CCTSETestLuaPluginDlg::~CCTSETestLuaPluginDlg(void)
    {
    }


    void CCTSETestLuaPluginDlg::CloseSubWndPlugin()
    {
        if (mpTestThread)
        {
            mpTestThread->TerminateMessageThread();
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    bool CCTSETestLuaPluginDlg::OnIdelTestStatus()
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

    bool CCTSETestLuaPluginDlg::OnBusyTestStatus()
    {
        mpDoMainList->HomeUp();
        mpDoMainList->SetAllUnSelectedItem();
        mpDoMainList->RemoveAllShow();
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);

        mpThroughtputTestEdt->SetText(L"");
        OnAppendRichEditTest(WPARAM(JGW_GetFormatWString(L"Is Save Test Log : %s LF\r...",mbIsSaveTestLog ? L"true" : L"false").c_str()),NULL);

        return true;
    }

    bool CCTSETestLuaPluginDlg::OnFailTestStatus()
    {
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");

        if (mbIsSaveTestLog) 
        {
            bool result = CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",false,mstrTestLogStream.str());
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"Save Test Log Result %s",result ? L"true" : L"false");
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
        }
        return true;
    }

    bool CCTSETestLuaPluginDlg::OnPassTestStatus()
    {
        if (mbIsSaveTestLog)
        {
            bool result = CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",true,mpThroughtputTestEdt->GetText().GetData());
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"Save Test Log Result %s",result ? L"true" : L"false");
            OnAppendRichEditTest(WPARAM(strTemp.c_str()),NULL);
        }
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_AUTO_TEST,WPARAM(NULL),0);
        return true;
    }

    void CCTSETestLuaPluginDlg::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
    }

    void CCTSETestLuaPluginDlg::OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (mpDoMainList)
        {
            mpDoMainList->SetNeedEndDown(true);
        }
    }
}
