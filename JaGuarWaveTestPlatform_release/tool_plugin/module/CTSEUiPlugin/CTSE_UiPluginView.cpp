#include "StdAfx.h"
#include "CTSE_UiPluginView.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "TSEUITestThread.h"
#include <JGW_TestLoggingPlugin/CJGW_SaveTestLog.h>
#include <JGW_FoundationFunc/CUtf8String.hpp>
#include "CJGW_TSEUIConfig.h"
#include <JGW_TestLoggingPlugin/CJGW_SaveTestLog.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CCTSE_UiPluginView::CCTSE_UiPluginView(void) : mpTestThread(NULL)
    {
        msPluginSkin.mstrSkinXmlPath = L"tes_ui_test_plugin.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
        mbIsShowLogHwnd = false;
        msTestConfigParam.mbInitSequenceTestOk = false;
    }

    CCTSE_UiPluginView::~CCTSE_UiPluginView(void)
    {
    }

    void CCTSE_UiPluginView::CloseSubWndPlugin()
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

    void CCTSE_UiPluginView::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
    }

    bool CCTSE_UiPluginView::OnIdelTestStatus()
    {
        //mpDoMainList = static_cast<CListTestCtrlUI*>(mpPluginPaintManagerUI->FindControl(L"domainlist"));
        mpThroughtputTestEdt = static_cast<CRichEditUI*>(mpPluginPaintManagerUI->FindControl(L"throughtputTestEdt"));
        mpUITestContent = static_cast<CHorizontalLayoutUI*>(mpPluginPaintManagerUI->FindControl(L"UITestContent"));
        if (!mpUITestContent) return false;
        /*{
            mpDoMainList->SetAttribute(L"itemalign",L"left");
            mpDoMainList->GetList()->RemoveAll();
            mpDoMainList->GetHeader()->SetScaleHeader(false);
            mpDoMainList->GetHeader()->RemoveAll();

            mpDoMainList->InsertColumn(0,_T("Index"),60);
            mpDoMainList->InsertColumn(1,_T("Test Project"),0);
            mpDoMainList->InsertColumn(2,_T("Content"),200);
            mpDoMainList->InsertColumn(3,_T("Time"),80);
            mpDoMainList->InsertColumn(4,_T("P/F"),40);
        }*/
        //! Init Param
        {
            msTestConfigParam.mhMainWnd = mpPluginPaintManagerUI->GetPaintWindow();
            msTestConfigParam.mstrSuiteXmlConfigPath = JGW_GetTSEConfigFilePath(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpViewResolverParam->mpPluginInfo->mstrSuiteXmlName);
        }

        {
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TSE_SHOW_CONTENT,&CCTSE_UiPluginView::OnTSEShowTestContent);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_LIST_MSG,&CCTSE_UiPluginView::OnTestListMsg);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_INIT_TSE_TEST_FAIL_MSG,&CCTSE_UiPluginView::OnTSEInitFail);
            ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_RICHEDIT_APPEND_TEST_MSG,&CCTSE_UiPluginView::OnAppendRichEditTest);

        }
        //! Init Test Thread
        {
            std::wstring strUIConfigPath;
            JGW_FormatWString(strUIConfigPath,L"%sui_config.ini",JGW_GetCacheConfigFolder(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName).c_str());
            CCJGW_TSEUIConfig tseUIConfig;
            tseUIConfig.LoadTSEUIConfig(strUIConfigPath,msUIConfigInfo);

            if (!LoadTSEUISkinXml(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName,JGW_W2A(msUIConfigInfo.mstrUISkinXmlFileName))) return false;

            if (!mpTestThread) mpTestThread = new CTSEUITestThread;
            if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

            return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&msTestConfigParam));
        }   

        return true;
    }

    bool CCTSE_UiPluginView::OnBusyTestStatus()
    {
        //mpDoMainList->HomeUp();
        //mpDoMainList->SetAllUnSelectedItem();
        //mpDoMainList->RemoveAllShow();
        //mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        ClearTSEUI();
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        mpThroughtputTestEdt->SetText(L"");
        return true;
    }

    bool CCTSE_UiPluginView::OnFailTestStatus()
    {
        //ClearTSEUI();
        //mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        //! KillTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO);
        return true;
    }

    bool CCTSE_UiPluginView::OnPassTestStatus()
    {
        //! KillTimer(mpPluginPaintManagerUI->GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO);
        UpdateTSEUI();
        CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",true,mpThroughtputTestEdt->GetText().GetData());
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_AUTO_TEST,WPARAM(NULL),0);
        return true;
    }

    /*std::wstring CCTSE_UiPluginView::GetSingleTestTime(double dTimer)
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
    }*/

    void CCTSE_UiPluginView::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        //_ASSERT(wParam);
        //_ASSERT(lParam);
        //DWORD nRgn = (WPARAM)lParam;
        //LPCTSTR strTestProject = (LPCTSTR)wParam;
        //CListTextElementUI* pListTextElementUi = NULL;
        //int  nIndex = mpDoMainList->GetCount();
        //std::wstring strTemp;

        //switch ( nRgn )
        //{
        //case BALCK:
        //    FormatWString(strTemp,L"%d",nIndex+1);
        //    pListTextElementUi = mpDoMainList->InsertItem( mpDoMainList->GetList()->GetCount(),strTemp.c_str(),30);
        //    mpDoMainList->SetNeedEndDown(true);
        //    pListTextElementUi->SetText(0,strTemp.c_str());
        //    pListTextElementUi->SetText(1,strTestProject);
        //    pListTextElementUi->SetText(2,L"");
        //    pListTextElementUi->SetText(3,L"---");
        //    pListTextElementUi->SetText(4,L"---");
        //    pListTextElementUi->Select(false);
        //    m_cHiperTime.Start();
        //    break;
        //case RED:
        //    pListTextElementUi = mpDoMainList->GetListTextElementItemClass(nIndex - 1);
        //    if(!pListTextElementUi) return ;
        //    m_cHiperTime.Stop();    
        //    //! pListTextElementUi->SetText(1,strTestProject);
        //    pListTextElementUi->SetText(3,GetSingleTestTime(m_cHiperTime.GetTimeMs()).c_str());
        //    pListTextElementUi->SetText(4,L"F");
        //    pListTextElementUi->Select(true);
        //    break;
        //case GREEN:
        //    pListTextElementUi = mpDoMainList->GetListTextElementItemClass(nIndex-1);
        //    if(!pListTextElementUi) return ;
        //    m_cHiperTime.Stop();
        //    pListTextElementUi->SetText(3,GetSingleTestTime(m_cHiperTime.GetTimeMs()).c_str());
        //    pListTextElementUi->SetText(4,L"P");
        //    pListTextElementUi->Select(false);
        //    break;
        //}  
    }

    void CCTSE_UiPluginView::OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam)
    {
        MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),GetString(E_TSE_INITFAIL_CHECKCONFIG_LOG),L"TSE INTI",MB_ICONERROR);
        PostMessage(msTestConfigParam.mhMainWnd,WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS,NULL,NULL);
    }

    void CCTSE_UiPluginView::OnTSEShowTestContent(const WPARAM& wParam,const LPARAM& lParam)
    {
        //CListTextElementUI* pListTextElementUi = mpDoMainList->GetListTextElementItemClass(mpDoMainList->GetCount() - 1);
        //if(!pListTextElementUi) return ;
        //pListTextElementUi->SetText(2,(LPCTSTR)wParam);
    }

    void CCTSE_UiPluginView::OnAppendRichEditTest(const WPARAM& wParam,const LPARAM& lParam)
    {
        std::wstring strTemp((const wchar_t*)wParam);
        JGW_EraseLastAndFristTwoCharsW(strTemp);
        mpThroughtputTestEdt->AppendText(strTemp.c_str());
        mpThroughtputTestEdt->AppendText(L"\r\n");
        mpThroughtputTestEdt->EndDown();
    }

    void CCTSE_UiPluginView::ClearTSEUI()
    {
        for (size_t i = 0;i < msUIConfigInfo.mvControlNames.size();i ++)
        {
            CControlUI* mpTempControlUI = static_cast<CControlUI*>(mpPluginPaintManagerUI->FindControl(msUIConfigInfo.mvControlNames[i].c_str()));
            if (mpTempControlUI)
            {
                mpTempControlUI->SetText(L"");
            }
        }
    }

    void CCTSE_UiPluginView::UpdateTSEUI()
    {
        std::wstring strBarcode,strStartBarcode,strEndBarcode = L"  ";
        for (size_t i = 0;i < msUIConfigInfo.mvControlNames.size();i ++)
        {
            CControlUI* mpTempControlUI = static_cast<CControlUI*>(mpPluginPaintManagerUI->FindControl(msUIConfigInfo.mvControlNames[i].c_str()));
            if (mpTempControlUI)
            {
                JGW_FormatWString(strStartBarcode,L"%s: ",msUIConfigInfo.mvControlNames[i].c_str());
                strBarcode = CCJGW_SaveTestLog::GetBarcodeToTestLog(mpThroughtputTestEdt->GetText().GetData(),strStartBarcode,strEndBarcode);
                mpTempControlUI->SetText(strBarcode.c_str());
            }
        }
    }

    bool CCTSE_UiPluginView::LoadTSEUISkinXml(const std::wstring& strModuleFolderName,const std::string& strUISkinXmlFileName)
    {
        std::string strUiConfigXml;
        JGW_FormatString(strUiConfigXml,"%s%s",JGW_W2A(JGW_GetCacheConfigFolder(strModuleFolderName)).c_str(),strUISkinXmlFileName.c_str());
        size_t nFileSize = 0;
        char* szbuf = JGW_GetFileBufToPath_C(strUiConfigXml.c_str(),nFileSize,"r");
        if (nFileSize <= 0 || NULL == szbuf)
        {
            return false;
        }
        std::wstring strTempUi = JGW_A2W_A(/*W(*/szbuf/*)*/);
        delete[] szbuf;

        CDialogBuilder builder;
        mpSubContainer = builder.Create(strTempUi.c_str(),NULL,NULL,mpPluginPaintManagerUI,mpUITestContent);
        if (!mpSubContainer)
        {
            //CTFLogWE_F(L"CCTFUiLibPluginViewResolver::CreateDuiLibSubWnd CDialogBuilder Create fail(mstrSkinXmlPath:%s)",L"WelcomDlg.xml");
            return false;
        }
        mpSubContainer->SetManager(mpPluginPaintManagerUI,mpUITestContent);
        return true;
    }

    //CCTSE_UiPluginView::CCTSE_UiPluginView(void)
    //{
    //    //msPluginSkin.mstrSkinXmlPath = L"tse_through_plugin_skin.xml";
    //    //msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    //    //mbIsShowLogHwnd = false;
    //}


    //CCTSE_UiPluginView::~CCTSE_UiPluginView(void)
    //{
    //     CloseSubWndPlugin();
    //}

    //bool CCTSE_UiPluginView::CreateSubWnd( LPS_SUB_WND_IMPL_PARAM psSubWndImplParam )
    //{
    //    mpPluginInfo = (LPS_PLUGIN_INFO)psSubWndImplParam->mpPluginInfo;
    //    mpParentContainer = (CContainerUI*)psSubWndImplParam->mpMainParentContainer;
    //    mpPaintManeger = (CPaintManagerUI*)psSubWndImplParam->mpMainPaintManager;

    //    if ( !mpPluginInfo || !mpPaintManeger || !mpParentContainer )
    //    {
    //        //CTFLogWE_F(L"CCWelcomViewDlg::CreateSubWnd子窗体初始化参数不能为空 mpPluginInfo:%d,mpPaintUi:%d,mpParentContainer:%d",mpPluginInfo,mpPaintManeger,mpParentContainer);
    //        return false;
    //    }

    //    std::wstring strUiConfigXml;
    //    JGW_FormatWString(strUiConfigXml,L"%sWelcomDlg.xml",JGW_GetCacheConfigFolder(mpPluginInfo->mstrModuleFolderName).c_str());
    //    size_t nFileSize = 0;
    //    char* szbuf = JGW_GetFileBufToPath_C(JGW_W2A(strUiConfigXml).c_str(),nFileSize,"r");
    //    if (nFileSize <= 0 || NULL == szbuf)
    //    {
    //        return false;
    //    }
    //    std::wstring strTempUi = JGW_A2W_A(szbuf);
    //    delete[] szbuf;

    //    CDialogBuilder builder;
    //    ::SendMessage( mpPaintManeger->GetPaintWindow(),WM_RESPONCE_PLUGIN_VERSION_MSG_DEFINE,WPARAM(SUB_PLUGIN_VERSION),NULL);
    //    mpSubContainer = builder.Create( strTempUi.c_str(),NULL,NULL,mpPaintManeger,mpParentContainer );
    //    if (!mpSubContainer)
    //    {
    //        //CTFLogWE_F(L"CCTFUiLibPluginViewResolver::CreateDuiLibSubWnd CDialogBuilder Create fail(mstrSkinXmlPath:%s)",L"WelcomDlg.xml");
    //        return false;
    //    }
    //    mpSubContainer->SetManager(mpPaintManeger,mpParentContainer);
    //    return true;
    //}

    //void CCTSE_UiPluginView::OnMsgNotify(LPVOID pNotifyMsg)
    //{

    //}

    //void CCTSE_UiPluginView::CloseSubWndPlugin()
    //{
    //    if ( mpSubContainer && mpParentContainer )
    //    {
    //        mpParentContainer->Remove(mpSubContainer);
    //        mpSubContainer = NULL;
    //    }
    //}
}
