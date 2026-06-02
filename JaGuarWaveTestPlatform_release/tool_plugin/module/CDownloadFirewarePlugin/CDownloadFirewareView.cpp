#include "StdAfx.h"
#include "CDownloadFirewareView.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CCDownloadFirewareView::CCDownloadFirewareView(void):mpTestThread(NULL)
    {
        msPluginSkin.mstrSkinXmlPath = L"richedit.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }


    CCDownloadFirewareView::~CCDownloadFirewareView(void)
    {
    }

    void CCDownloadFirewareView::CloseSubWndPlugin()
    {
        if ( mpTestThread )
        {
            delete mpTestThread;
            mpTestThread = NULL;
        }
       CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    bool CCDownloadFirewareView::OnIdelTestStatus()
    {
        mpDoMainRichEditUi = static_cast<CRichEditUI*>(mpPluginPaintManagerUI->FindControl(L"domainedt"));
        if (!mpDoMainRichEditUi) return false;

        JGW_GetModuleWorkPath(msDownloadFirewareParam.mstrFirewareFolder);
        msDownloadFirewareParam.mstrFirewareFolder += L"dongles\\";

        msDownloadFirewareParam.mstrFirewareToolFilePath = msDownloadFirewareParam.mstrFirewareFolder + DOWNLOAD_FIREWARE_TOOL_NAME;

        JGW_GetModuleWorkPath(msDownloadFirewareParam.mstrCMDFilePath);
        msDownloadFirewareParam.mstrCMDFilePath += L"cmd.exe";

        msDownloadFirewareParam.mhMainWnd = mpViewResolverParam->mpMainPaintManager->GetPaintWindow();
        //! WM_HWND_TEST_LIST_MSG
        ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_RICHEDIT_APPEND_TEST_MSG,&CCDownloadFirewareView::OnRichEditAppendTestMsg);


        if (!mpTestThread) mpTestThread = new CCDownloadFirewareThread;
        if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

        return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&msDownloadFirewareParam));
    }

    bool CCDownloadFirewareView::OnBusyTestStatus()
    {
        mpDoMainRichEditUi->SetText(L"");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        return true;
    }

    bool CCDownloadFirewareView::OnFailTestStatus()
    {
        return true;
    }

    bool CCDownloadFirewareView::OnPassTestStatus()
    {
        return true;
    }

    void CCDownloadFirewareView::OnRichEditAppendTestMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (wParam) mpDoMainRichEditUi->AppendText((const wchar_t*)wParam);
        mpDoMainRichEditUi->EndDown();
    }
}
