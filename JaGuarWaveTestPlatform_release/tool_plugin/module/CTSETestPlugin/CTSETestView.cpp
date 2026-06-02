#include "StdAfx.h"
#include "CTSETestView.h"
#include "TSETestThread.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CCTSETestView::CCTSETestView(void) : mpTestThread(NULL),mnTestIndex(1),mnErrorTestIndex(-1),mnMaxTestIndex(0)
    {
        msPluginSkin.mstrSkinXmlPath = L"test.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
        msTSETestConfigParam.mbInitSequenceTestOk = false;
        mvTestContentButton.reserve(100);
    }

    CCTSETestView::~CCTSETestView(void)
    {
    }

    void CCTSETestView::CloseSubWndPlugin()
    {
#if 1
        mvTestContentButton.clear();
        if (mpDoMainList)
        {
            for (int i = 0;i < mpDoMainList->GetCount();i ++)
            {
                CListContainerElementUIEx* pListTextElementUi = mpDoMainList->GetCListContainerElementUIEx(i);
                ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(1))->GetItemAt(0)->OnNotify -= MakeDelegate(this,&CCTSETestView::OnMsgBtnClick,_T("click"));
            }
        } 
#endif 
        if (mpTestThread)
        {
            mpTestThread->TerminateMessageThread();
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    void CCTSETestView::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
    }

    bool CCTSETestView::OnIdelTestStatus()
    {
        mpDoMainList = static_cast<CListTestCtrlUI*>(mpPluginPaintManagerUI->FindControl(L"domainlist"));
        if (!mpDoMainList) return false;
        {
            mpDoMainList->SetAttribute(L"itemalign",L"left");
            mpDoMainList->GetList()->RemoveAll();
            mpDoMainList->GetHeader()->SetScaleHeader(false);
            mpDoMainList->GetHeader()->RemoveAll();

            mpDoMainList->InsertColumn(0,_T("Index"),60);
            mpDoMainList->InsertColumn(1,_T("Test Project"),0);
            mpDoMainList->InsertColumn(2,_T("Content"),0);
            mpDoMainList->InsertColumn(3,_T("Time"),80);
            mpDoMainList->InsertColumn(4,_T("P/F"),40);
        }
        //! Init Param
        {
            msTSETestConfigParam.mhMainWnd = mpPluginPaintManagerUI->GetPaintWindow();
            msTSETestConfigParam.mstrSuiteXmlConfigPath = JGW_GetTSEConfigFilePath(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpViewResolverParam->mpPluginInfo->mstrSuiteXmlName);
        }
        {
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TSE_SHOW_CONTENT,&CCTSETestView::OnTSEShowTestContent);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_LIST_MSG,&CCTSETestView::OnTestListMsg);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_INIT_TSE_TEST_FAIL_MSG,&CCTSETestView::OnTSEInitFail);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TSE_INDEX_MSG,&CCTSETestView::OnTSETestIndexMsg);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_RESET_TSE_COMPLETE,&CCTSETestView::OnTSEResetComplete);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TSE_INDEX_MSG_RUN_COMPLETE,&CCTSETestView::OnTSEIndexTestComplete);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_PAINT,&CCTSETestView::OnPaintList);
        }
        //! Init Test Thread
        {
            if (!mpTestThread) mpTestThread = new CTSETestThread;
            if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

            return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&msTSETestConfigParam));
        }   
    }

    void CCTSETestView::OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (mpDoMainList)
        {
            mpDoMainList->SetNeedEndDown(true);
        }
    }

    void CCTSETestView::InitStartTest()
    {
        mnErrorTestIndex = -1;
        mnMaxTestIndex = 0;
        msTSETestConfigParam.mbInitSequenceTestOk = true;
        mpDoMainList->HomeUp();
        mpDoMainList->SetAllUnSelectedItem();
        EnableAllButton(false);
        for (int i = 0;i < mpDoMainList->GetCount();i ++)
        {
            CListContainerElementUIEx* pListTextElementUi = mpDoMainList->GetCListContainerElementUIEx(i);
            pListTextElementUi->SetBkColor(0xFFE2DDDF);
            ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(2))->GetItemAt(0)->SetText(L"");
            ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(3))->GetItemAt(0)->SetText(L"---");
            ((CHorizontalLayoutUI*)pListTextElementUi->GetItemAt(4))->GetItemAt(0)->SetText(L"---");
        }
    }
    //! FFE2DDDF
    bool CCTSETestView::OnBusyTestStatus()
    {
        InitStartTest();
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        return true;
    }

    bool CCTSETestView::OnPassTestStatus()
    {
        EnableAllButton(true);
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_AUTO_TEST,WPARAM(NULL),0);
        return true;
    }

    bool CCTSETestView::OnFailTestStatus()
    {
        EnableAllButton(true);
        return true;
    }

    void CCTSETestView::OnTSEShowTestContent(const WPARAM& wParam,const LPARAM& lParam)
    {
        CListContainerElementUIEx* pListTextElementUi = (CListContainerElementUIEx*)mpDoMainList->GetListTextElementItemClass(mnTestIndex);
        if(!pListTextElementUi) return ;
        CHorizontalLayoutUI* pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(pListTextElementUi->GetItemAt(2));
        pHorizontalLayoutUi->GetItemAt(0)->SetText((const wchar_t*)wParam);
    }

    std::wstring CCTSETestView::GetSingleTestTime(double dTimer)
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

    void CCTSETestView::OnTSETestIndexMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mnTestIndex = (size_t)wParam;
    }

    void CCTSETestView::OnTSEResetComplete(const WPARAM& wParam,const LPARAM& lParam)
    {
        InitStartTest();
        EnableAllButton(true);
    }

    bool CCTSETestView::OnMsgBtnClick( TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam )
    {
        size_t nCurrentTestIndex = (size_t)pTNotifyUI->pSender->GetDate();
        if (nCurrentTestIndex >= mnMaxTestIndex) 
        {
            mnMaxTestIndex = nCurrentTestIndex;
        }
        else 
        {
            MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),GetString(E_TSE_TEST_INDEX_RULE),GetString(E_PROMPT_TYPE),MB_ICONERROR);
            return false;
        }

        mnErrorTestIndex = -1;
        msTSETestConfigParam.mbInitSequenceTestOk = true;
        EnableAllButton(false);
        mpTestThread->PostThreadMessage((mpDoMainList->GetCount() == nCurrentTestIndex + 1)?WM_TEST_THREAD_RESET_TSE:WM_TEST_THREAD_TSE_INDEX_MSG_RUN,WPARAM(nCurrentTestIndex));    
        return true;
    }

    void CCTSETestView::OnTSEIndexTestComplete(const WPARAM& wParam,const LPARAM& lParam)
    {
        EnableAllButton(true);
    }

    void CCTSETestView::EnableAllButton(bool bEnable /* = false */)
    {
        for (int i = 0;i < mvTestContentButton.size();i ++)
        {
            mvTestContentButton[i]->SetEnabled(bEnable);
        }
    }

    void CCTSETestView::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        CListContainerElementUIEx* plsitContainerItem = NULL;
        plsitContainerItem =  mpDoMainList->GetCListContainerElementUIEx(mnTestIndex);
        if (msTSETestConfigParam.mbInitSequenceTestOk && !plsitContainerItem) return ;

        switch (lParam)
        {
        case BALCK:
            //! INIT ³õÊ¼»¯TSE
            if (!msTSETestConfigParam.mbInitSequenceTestOk) 
            {
                std::wstring strTemp;
                FormatWString(strTemp,L"%d",mpDoMainList->GetCount() + 1);
                plsitContainerItem = mpDoMainList->InsertContainerItem(_T("sigle_list_item_test.xml"),40);
                if (!plsitContainerItem) return ;
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(0))->GetItemAt(0)->SetText(strTemp.c_str());
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetText((LPCTSTR)wParam);
#if 1
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->SetDate((LPVOID)(mpDoMainList->GetCount() - 1));
                mvTestContentButton.push_back(static_cast<CButtonUI*>(((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)));
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(1))->GetItemAt(0)->OnNotify += MakeDelegate(this,&CCTSETestView::OnMsgBtnClick,_T("click"));
#endif
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(L"");
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->GetItemAt(0)->SetText(L"---");
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(4))->GetItemAt(0)->SetText(L"---");
                mpDoMainList->SetNeedEndDown(true);
            }
            else
            {
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(L"");
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->GetItemAt(0)->SetText(L"---");
                ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(4))->GetItemAt(0)->SetText(L"R");
                mpDoMainList->LineDown(mnTestIndex,40);
                plsitContainerItem->SetBkColor(0xFF16FF00);
                m_cHiperTime.Start();
            } 
            break;
        case BULE:
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(2))->GetItemAt(0)->SetText(L"");
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->GetItemAt(0)->SetText(L"---");
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(4))->GetItemAt(0)->SetText(L"R");
            plsitContainerItem->SetBkColor(0xFF16FF00);
            if (mnTestIndex == mnErrorTestIndex) mnErrorTestIndex = -1;
            break;
        case RED:
            m_cHiperTime.Stop();    
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->GetItemAt(0)->SetText(GetSingleTestTime(m_cHiperTime.GetTimeMs()).c_str());
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(4))->GetItemAt(0)->SetText(L"F");
            plsitContainerItem->Select(true);
            plsitContainerItem->SetBkColor(0xffff0000);
            if (-1 == mnErrorTestIndex) mnErrorTestIndex = mnTestIndex;
            break;
        case GREEN:
            m_cHiperTime.Stop();
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(3))->SetText(GetSingleTestTime(m_cHiperTime.GetTimeMs()).c_str());
            ((CHorizontalLayoutUI*)plsitContainerItem->GetItemAt(4))->GetItemAt(0)->SetText(L"P");
            plsitContainerItem->SetBkColor(0xFFE9F5FF);
            if (mnTestIndex == mnErrorTestIndex) mnErrorTestIndex = -1;
            break;
        }    
    }

    void CCTSETestView::OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam)
    {
        MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),GetString(E_TSE_INITFAIL_CHECKCONFIG_LOG),L"TSE INTI",MB_ICONERROR);
        PostMessage(msTSETestConfigParam.mhMainWnd,WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS,NULL,NULL);
    }

    void CCTSETestView::OnPaintList(const WPARAM& wParam,const LPARAM& lParam)
    {
        if (!mpDoMainList) return ;
        static int gnPrevListHeight = 0;
        if (gnPrevListHeight == mpDoMainList->GetHeight() || -1 == mnErrorTestIndex) return ;
        gnPrevListHeight = mpDoMainList->GetHeight();
        mpDoMainList->LineDown(mnErrorTestIndex,40);
    }
}