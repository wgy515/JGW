#include "StdAfx.h"
#include "CRFCalVerTestView.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CCRFCalVerTestView::CCRFCalVerTestView(void):mpTestThread(NULL),m_nListItemHeight(30),mpDoMainList(NULL)
    {
        msPluginSkin.mstrSkinXmlPath = L"test.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }


    CCRFCalVerTestView::~CCRFCalVerTestView(void)
    {
    }

    void CCRFCalVerTestView::CloseSubWndPlugin()
    {
        if (mpTestThread)
        {
            mpTestThread->TerminateMessageThread();
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    bool CCRFCalVerTestView::OnIdelTestStatus()
    {
        mpDoMainList = static_cast<CListTestCtrlUI*>(mpPluginPaintManagerUI->FindControl(L"domainlist"));
        if (!mpDoMainList) return false;

        //! INIT UI 
        {
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
            msRFCalVerTestParam.mhMainWnd = mpViewResolverParam->mpMainPaintManager->GetPaintWindow();
            msRFCalVerTestParam.mstrConfigXmlPath = JGW_GetTSEConfigFilePath(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpViewResolverParam->mpPluginInfo->mstrSuiteXmlName);
             /*msRFCalVerTestParam.mstrConfigXmlPath = JGW_GetApplicationFolder();
             msRFCalVerTestParam.mstrConfigXmlPath += L"cache\\";
             msRFCalVerTestParam.mstrConfigXmlPath += mpViewResolverParam->mpPluginInfo->mstrModuleFolderName;
             msRFCalVerTestParam.mstrConfigXmlPath += L"\\suite.xml";*/
        }

        {
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TSE_SHOW_CONTENT,&CCRFCalVerTestView::OnTSEShowTestContent);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_LIST_MSG,&CCRFCalVerTestView::OnTestListMsg);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_INIT_TSE_TEST_FAIL_MSG,&CCRFCalVerTestView::OnTSEInitFail);
        }
        //! Init Test Thread
        {
            if (!mpTestThread) mpTestThread = new CCRFCalVerTestThread;
            if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

            return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&msRFCalVerTestParam));
        }   
    }

    void CCRFCalVerTestView::OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (mpDoMainList)
        {
            mpDoMainList->SetNeedEndDown(true);
        }
    }

    void CCRFCalVerTestView::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
    }

    void CCRFCalVerTestView::OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam)
    {
        MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),L"初始化配置失败，请检查配置是否正常,详情见Log",L"TSE INTI",MB_ICONERROR);
        PostMessage(msRFCalVerTestParam.mhMainWnd,WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS,NULL,NULL);
    }

    bool CCRFCalVerTestView::OnBusyTestStatus()
    {
        mpDoMainList->HomeUp();
        mpDoMainList->SetAllUnSelectedItem();
        mpDoMainList->RemoveAllShow();
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        return true;
    }

    bool CCRFCalVerTestView::OnFailTestStatus()
    {
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        return true;
    }

    bool CCRFCalVerTestView::OnPassTestStatus()
    {
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_AUTO_TEST,WPARAM(NULL),0);
        return true;
    }

    std::wstring CCRFCalVerTestView::GetSingleTestTime(double dTimer)
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

    void CCRFCalVerTestView::OnTSEShowTestContent(const WPARAM& wParam,const LPARAM& lParam)
    {
        CListTextElementUI* pListTextElementUi = mpDoMainList->GetListTextElementItemClass(mpDoMainList->GetCount() - 1);
        if(!pListTextElementUi) return ;
        pListTextElementUi->SetText(2,(LPCTSTR)wParam);
    }

    void CCRFCalVerTestView::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
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