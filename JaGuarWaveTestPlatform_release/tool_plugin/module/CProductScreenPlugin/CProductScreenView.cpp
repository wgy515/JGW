#include "StdAfx.h"
#include "CProductScreenView.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CCProductScreenView::CCProductScreenView(void):mpTestThread(NULL)
    {
        msPluginSkin.mstrSkinXmlPath = L"richedit.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
        mbIsShowLogHwnd = false;
    }


    CCProductScreenView::~CCProductScreenView(void)
    {
    }

    void CCProductScreenView::CloseSubWndPlugin()
    {
        if ( mpTestThread )
        {
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    bool CCProductScreenView::OnIdelTestStatus()
    {
        mpDoMainRichEditUi = static_cast<CRichEditUI*>(mpPluginPaintManagerUI->FindControl(L"domainedt"));
        if (!mpDoMainRichEditUi) return false;

        JGW_GetModuleWorkPathA(msProductScreenParam.mstrPDBImageFolder);
        msProductScreenParam.mstrPDBImageFolder += "dongles\\";
        JGW_GetExecSubFolder(msProductScreenParam.mstrProductScreenToolFolder,L"dongles");
        msProductScreenParam.mstrReportXmlFilePath = msProductScreenParam.mstrProductScreenToolFolder + _T(PRODUCT_SCREEN_XML_REPORT_FILE);
        msProductScreenParam.mstrTestResultFilePath = msProductScreenParam.mstrProductScreenToolFolder + _T(PRODUCT_SCREEN_TEST_RESULT_TXT);
        msProductScreenParam.mstrVerboseLogFilePath = msProductScreenParam.mstrProductScreenToolFolder + _T(PRODUCT_SCREEN_VERBOSE_LOG_FILE);
        JGW_GetExecSubFolder(msProductScreenParam.mstrTestLogFolder,L"test_log");
        JGW_GetModuleWorkPath(msProductScreenParam.mstrCMDFilePath);
        msProductScreenParam.mstrCMDFilePath += L"cmd.exe";
        msProductScreenParam.mhMainWnd = mpViewResolverParam->mpMainPaintManager->GetPaintWindow();
        //! WM_HWND_TEST_LIST_MSG
        ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_RICHEDIT_APPEND_TEST_MSG,&CCProductScreenView::OnRichEditAppendTestMsg);


        if (!mpTestThread) mpTestThread = new CCProductScreenThread;
        if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

        return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&msProductScreenParam));
    }

    bool CCProductScreenView::OnBusyTestStatus()
    {
        mpDoMainRichEditUi->SetText(L"");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        return true;
    }

    bool CCProductScreenView::OnFailTestStatus()
    {
        return true;
    }

    bool CCProductScreenView::OnPassTestStatus()
    {
        return true;
    }

    void CCProductScreenView::OnRichEditAppendTestMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (wParam) mpDoMainRichEditUi->AppendText((const wchar_t*)wParam);
        mpDoMainRichEditUi->EndDown();
    }
}
