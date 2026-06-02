#include "StdAfx.h"
#include "CWiFiTestView.h"
#include "JGWWiFiTestConfig.h"
#include <fstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_TestLoggingPlugin/CJGW_MoveTestLogging.h>

namespace JGW
{
    CCWiFiTestView::CCWiFiTestView(void):mpTestThread(NULL),m_nListItemHeight(30)
    {
        msPluginSkin.mstrSkinXmlPath = L"test.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }


    CCWiFiTestView::~CCWiFiTestView(void)
    {
        CloseSubWndPlugin();
    }

    void CCWiFiTestView::CloseSubWndPlugin()
    {
        if ( mpTestThread )
        {
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    bool CCWiFiTestView::OnIdelTestStatus()
    {
        mpDoMainList = static_cast<CListTestCtrlUI*>(mpPluginPaintManagerUI->FindControl(L"domainlist"));
        if (!mpDoMainList) return false;

        //! INIT UI 
        {
            mpDoMainList->SetAttribute(L"itemalign",L"center");
            mpDoMainList->GetList()->RemoveAll();
            mpDoMainList->GetHeader()->SetScaleHeader(false);

            mpDoMainList->GetHeader()->GetItemAt( 0 )->SetText(_T("²âÊÔÏî"));
            mpDoMainList->GetHeader()->GetItemAt( 0 )->SetAttribute(L"align",L"center");
            mpDoMainList->GetHeader()->GetItemAt( 1 )->SetText(_T("Min"));
            mpDoMainList->GetHeader()->GetItemAt( 1 )->SetAttribute(L"align",L"center");
            mpDoMainList->GetHeader()->GetItemAt( 2 )->SetText(_T("Value"));
            mpDoMainList->GetHeader()->GetItemAt( 2 )->SetAttribute(L"align",L"center");
            mpDoMainList->InsertColumn(3,_T("Max"),0);
            mpDoMainList->InsertColumn(4,_T("P/F"),0);

            mpDoMainList->SetHeaderRowWeight(0,0);
            mpDoMainList->SetHeaderRowWeight(1,200);
            mpDoMainList->SetHeaderRowWeight(2,200);
            mpDoMainList->SetHeaderRowWeight(3,200);
            mpDoMainList->SetHeaderRowWeight(4,40);
//             mpDoMainList->GetHeader()->GetItemAt( 0 )->SetScale( 30 );
//             mpDoMainList->GetHeader()->GetItemAt( 1 )->SetScale( 20 );
//             mpDoMainList->GetHeader()->GetItemAt( 2 )->SetScale( 20 );
//             mpDoMainList->GetHeader()->GetItemAt( 3 )->SetScale( 20 );
//             mpDoMainList->GetHeader()->GetItemAt( 4 )->SetScale( 10 );
        }

        {
            m_sWiFiTestConfig.mhMainWnd = mpViewResolverParam->mpMainPaintManager->GetPaintWindow();
            JGWWiFiTestConfig wifiTestConfig;
            std::string strFilePath;
            std::string strCacheConfigFilePath = "wifi_param.xml";
            std::string strConfigFilePath = "wifi_config.xml";
            std::wstring strConfigIniFilePath;

            JGW_GetModuleWorkPathA(strFilePath);
            JGW_FormatString(strCacheConfigFilePath,"%scache\\%s\\wifi_param.xml",strFilePath.c_str(),JGW_W2A(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName).c_str());
            JGW_FormatString(strConfigFilePath,"%sconfig\\%s\\wifi_config.xml",strFilePath.c_str(),JGW_W2A(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName).c_str());

            JGW_FormatWString(strConfigIniFilePath,L"%sconfig\\%s\\equipconfig.ini",JGW_GetApplicationFolder(),mpViewResolverParam->mpPluginInfo->mstrModuleFolderName.c_str());
            //! strCacheConfigFilePath = strFilePath + "cache\\JGWWiFiTest\\wifi_param.xml";
            //! strConfigFilePath = strFilePath + "config\\JGWWiFiTest\\wifi_config.xml";

            if (!wifiTestConfig.LoadWiFiTestConfigXML(m_sWiFiTestConfig,strConfigFilePath)
                || !wifiTestConfig.LoadWiFiTestCacheXML(m_sWiFiTestConfig,strCacheConfigFilePath)
                || !wifiTestConfig.LoadMonitorConfig(strConfigIniFilePath,m_sWiFiTestConfig))
            {
                MessageBox(m_sWiFiTestConfig.mhMainWnd,_T("ÅäÖÃÎÄ¼þ¼ÓÔØÊ§°Ü!!!"),_T("³õÊ¼»¯Ê§°Ü"),MB_ICONERROR);
                return false;
            }
        }
        
        //! WM_HWND_TEST_LIST_MSG
        ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_LIST_MSG,&CCWiFiTestView::OnTestListMsg);

        if (!mpTestThread) mpTestThread = new CCWiFiTestThread;
        if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

        return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&m_sWiFiTestConfig));
    }

    bool CCWiFiTestView::OnBusyTestStatus()
    {
        mpDoMainList->HomeUp();
        mpDoMainList->SetAllUnSelectedItem();
        mpDoMainList->RemoveAllShow();
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        return true;
    }

    bool CCWiFiTestView::OnFailTestStatus()
    {
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        OnSaveTest(false);
        return true;
    }

    bool CCWiFiTestView::OnPassTestStatus()
    {
        OnSaveTest(true);
        return true;
    }

    void CCWiFiTestView::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        const wchar_t* strListCtrlMsg = (const wchar_t*)wParam;
        bool bInsertItem = (1 == lParam);
        static std::vector<std::wstring> _vListCtrlMsg;
        _vListCtrlMsg.clear();
        CListTextElementUI* m_pListTextElementUi = NULL;
        //mpDoMainList->LineDown();

        JGW_ParserStrW(strListCtrlMsg,_T("@"),_vListCtrlMsg);
        if ( bInsertItem )
        {
            m_pListTextElementUi = mpDoMainList->InsertItem( mpDoMainList->GetList()->GetCount(),_vListCtrlMsg.at(0).c_str(),m_nListItemHeight);
            mpDoMainList->SetNeedEndDown(true);
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
    }

    void CCWiFiTestView::OnSaveTest(bool bTestStatus)
    {
        std::wofstream fileStream;
        std::wstring strExcelContent;
        CListTextElementUI* pListTextElementUi = NULL;

        std::wstring strTemp,strFilePath;
        JGW_GetExecSubFolder(strTemp,L"log");
        JGW_FormatWString(strFilePath,L"%s%u.log",strTemp.c_str(),JGW_GetTimeOfDay());
        //! GetTickCount
        fileStream.imbue( std::locale("") );
        fileStream.open( strFilePath.c_str(),std::ios_base::out | std::ios_base::trunc );

        if (!fileStream.is_open()) return ;

        fileStream << L"²âÊÔÏî\tMin\tValue\tMax\tP/F" << std::endl;

        for (int i = 0;i < mpDoMainList->GetList()->GetCount();i ++)
        {
            pListTextElementUi = (CListTextElementUI*)mpDoMainList->GetList()->GetItemAt(i);

            for (int index = 0;index < 5;index ++)
            {
                fileStream << pListTextElementUi->GetText(index) << L"\t";
            }
            fileStream <<  std::endl;
        }

        fileStream.flush();
        fileStream.close();

        CCJGW_MoveTestLogging moveTestLogging;
        moveTestLogging.MoveTestLogging(L"WIFI_TEST\\",mpTestThread->GetSerialNumber().c_str(),strFilePath.c_str(),bTestStatus);

        return ;
    }
}
