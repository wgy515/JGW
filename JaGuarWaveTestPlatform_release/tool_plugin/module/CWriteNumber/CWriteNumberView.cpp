#include "StdAfx.h"
#include "CWriteNumberView.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CCWriteNumberView::CCWriteNumberView(void):mpTestThread(NULL),m_nListItemHeight(30)
    {
        msPluginSkin.mstrSkinXmlPath = L"test.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }


    CCWriteNumberView::~CCWriteNumberView(void)
    {
    }

    void CCWriteNumberView::CloseSubWndPlugin()
    {
        if ( mpTestThread )
        {
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    bool CCWriteNumberView::OnIdelTestStatus()
    {
        mpDoMainList = static_cast<CListTestCtrlUI*>(mpPluginPaintManagerUI->FindControl(L"domainlist"));
        if (!mpDoMainList) return false;

        JGW_GetModuleWorkPathA(msDownloadPDBParam.mstrFolder);
        msDownloadPDBParam.mstrFolder += "dongles\\";
        msDownloadPDBParam.mhMainWnd = mpViewResolverParam->mpMainPaintManager->GetPaintWindow();
        //! WM_HWND_TEST_LIST_MSG
        ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_LIST_MSG,&CCWriteNumberView::OnTestListMsg);


        if (!mpTestThread) mpTestThread = new CCWriteNumberThread;
        if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

        return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&msDownloadPDBParam));
    }

    bool CCWriteNumberView::OnBusyTestStatus()
    {
        mpDoMainList->HomeUp();
        mpDoMainList->SetAllUnSelectedItem();
        mpDoMainList->RemoveAllShow();
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        return true;
    }

    bool CCWriteNumberView::OnFailTestStatus()
    {
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        return true;
    }

    bool CCWriteNumberView::OnPassTestStatus()
    {
        return true;
    }

    void CCWriteNumberView::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        const wchar_t* strListCtrlMsg = (const wchar_t*)wParam;
        bool bInsertItem = (1 == lParam);
        static std::vector<std::wstring> _vListCtrlMsg;
        _vListCtrlMsg.clear();
        CListTextElementUI* m_pListTextElementUi = NULL;
        //m_pListCtrlUi->LineDown();

        JGW_ParserStrW(strListCtrlMsg,_T("@"),_vListCtrlMsg);
        if ( bInsertItem )
        {
            m_pListTextElementUi = mpDoMainList->InsertItem( mpDoMainList->GetList()->GetCount(),_vListCtrlMsg.at(0).c_str(),m_nListItemHeight);
        }
        else
        {
            m_pListTextElementUi = mpDoMainList->GetListTextElementItemClass( mpDoMainList->GetList()->GetCount() - 1);
        }
        if( !m_pListTextElementUi ) return ;

        for (size_t i = 0;i < _vListCtrlMsg.size();i ++)
        {
            m_pListTextElementUi->SetText(i,_vListCtrlMsg.at(i).c_str());
        }

        if ( std::wstring::npos != _vListCtrlMsg.at(_vListCtrlMsg.size() - 1).find(_T('F')) )
        {
            m_pListTextElementUi->Select();
            mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        }

        mpDoMainList->EndItemDown();
    }
}

