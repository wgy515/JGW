#include "StdAfx.h"
#include "CJGW_WiFiCalView.h"
#include <fstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_TestLoggingPlugin/CJGW_MoveTestLogging.h>

#include "CStationCalDataFile.h"
namespace JGW
{
    CCJGW_WiFiCalView::CCJGW_WiFiCalView(void):mpTestThread(NULL),m_nListItemHeight(30)
    {
        msPluginSkin.mstrSkinXmlPath = L"test.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
        msWiFiCalConfig.mnnBaudRate = 115200/*1500000*/;
        msWiFiCalConfig.mstrSerailComPort = L"";
        msWiFiCalConfig.mstrUserName = "root";
        msWiFiCalConfig.mstrPassword = "20171015";
        msWiFiCalConfig.mstrCurrentHostIPAddr = "192.168.1.5";
        msWiFiCalConfig.mstrServiceIPAddr = "192.168.1.1";
        msWiFiCalConfig.mstrWlan5GMac = "00.03.7f.11.22.AB";
        msWiFiCalConfig.mstrWlan2_4GMac = "00.03.7f.11.22.DC";
        //!
#ifdef _DEBUG
        msWiFiCalConfig.mstreepFName = "C:\\JW\\exec\\bdwlan.b290";
#else
        msWiFiCalConfig.mstreepFName = JGW_W2A(JGW_GetExecConfigFilePath(L"bdwlan.b290"));
#endif
        
        JGW_FormatString(msWiFiCalConfig.mstrWiFiCalSerailCommand ,"wifi down \r\nrmmod wifi_3_0\r\nrmmod wifi_2_0\r\nrmmod qca_ol\r\nsleep 2\r\ninsmod qca_ol hw_mode_id=1 testmode=1 cfg80211_config=1\r\nsleep 1\r\ninsmod wifi_3_0\r\nsleep 1\r\ndiag_socket_app -a %s &\r\nsleep 1\r\n/etc/init.d/ftm start\r\nsleep 1\r\n/usr/sbin/ftm -n -dd &\r\n",msWiFiCalConfig.mstrCurrentHostIPAddr.c_str());

        msWiFiCalConfig.ms5GCalParam.meTpcCalScheme = ALL_POINTS;
        msWiFiCalConfig.ms5GCalParam.meInstance = WLAN0;
        msWiFiCalConfig.ms5GCalParam.meUniversalPhyID = PHY_A0;
        msWiFiCalConfig.ms5GCalParam.mvChainToCalibrate.push_back(0);
        msWiFiCalConfig.ms5GCalParam.mvChainToCalibrate.push_back(1);
        msWiFiCalConfig.ms5GCalParam.mvChainToCalibrate.push_back(2);
        msWiFiCalConfig.ms5GCalParam.mvChainToCalibrate.push_back(3);
        msWiFiCalConfig.ms5GCalParam.mstrWlanNoiseFloorCalChannel = "5180,5320,5500,5745";

        msWiFiCalConfig.ms2GCalParam.meTpcCalScheme = ALL_POINTS;
        msWiFiCalConfig.ms2GCalParam.meInstance = WLAN1;
        msWiFiCalConfig.ms2GCalParam.meUniversalPhyID = PHY_B;
        msWiFiCalConfig.ms2GCalParam.mvChainToCalibrate.push_back(0);
        msWiFiCalConfig.ms2GCalParam.mvChainToCalibrate.push_back(1);
        msWiFiCalConfig.ms2GCalParam.mvChainToCalibrate.push_back(2);
        msWiFiCalConfig.ms2GCalParam.mvChainToCalibrate.push_back(3);
    }


    CCJGW_WiFiCalView::~CCJGW_WiFiCalView(void)
    {
        CloseSubWndPlugin();
    }

    void CCJGW_WiFiCalView::CloseSubWndPlugin()
    {
        if ( mpTestThread )
        {
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    bool CCJGW_WiFiCalView::OnIdelTestStatus()
    {
        mpDoMainList = static_cast<CListTestCtrlUI*>(mpPluginPaintManagerUI->FindControl(L"domainlist"));
        if (!mpDoMainList) return false;

        //! INIT UI 
        {
            mpDoMainList->SetAttribute(L"itemalign",L"left");
            mpDoMainList->GetList()->RemoveAll();
            mpDoMainList->GetHeader()->SetScaleHeader(false);

            mpDoMainList->GetHeader()->GetItemAt( 0 )->SetText(_T("校准项"));
            mpDoMainList->GetHeader()->GetItemAt( 0 )->SetAttribute(L"align",L"center");
            mpDoMainList->GetHeader()->GetItemAt( 1 )->SetText(_T("Min"));
            mpDoMainList->GetHeader()->GetItemAt( 1 )->SetAttribute(L"align",L"center");
            mpDoMainList->GetHeader()->GetItemAt( 2 )->SetText(_T("Value"));
            mpDoMainList->GetHeader()->GetItemAt( 2 )->SetAttribute(L"align",L"center");
            mpDoMainList->InsertColumn(3,_T("Max"),0);
            mpDoMainList->InsertColumn(4,_T("P/F"),0);
            mpDoMainList->InsertColumn(5,_T(""),0);

            mpDoMainList->SetHeaderRowWeight(0,0);
            mpDoMainList->SetHeaderRowWeight(1,80);
            mpDoMainList->SetHeaderRowWeight(2,80);
            mpDoMainList->SetHeaderRowWeight(3,80);
            mpDoMainList->SetHeaderRowWeight(4,40);
            mpDoMainList->SetHeaderRowWeight(5,20);
            //             mpDoMainList->GetHeader()->GetItemAt( 0 )->SetScale( 30 );
            //             mpDoMainList->GetHeader()->GetItemAt( 1 )->SetScale( 20 );
            //             mpDoMainList->GetHeader()->GetItemAt( 2 )->SetScale( 20 );
            //             mpDoMainList->GetHeader()->GetItemAt( 3 )->SetScale( 20 );
            //             mpDoMainList->GetHeader()->GetItemAt( 4 )->SetScale( 10 );
        }

        {
            CCStationCalDataFile stationCalDataFile;
            std::string strCalDataFilePath ;
            JGW_GetModuleWorkPathA(strCalDataFilePath);
            strCalDataFilePath += "StationCalDataFile_1dB.XML";
            stationCalDataFile.LoadStationCalDataFile(strCalDataFilePath,msWiFiCalConfig.mvsStationCalPath);
            //m_sWiFiTestConfig.mhMainWnd = mpViewResolverParam->mpMainPaintManager->GetPaintWindow();
            //JGWWiFiTestConfig wifiTestConfig;
            //std::string strFilePath;
            //std::string strCacheConfigFilePath = "wifi_param.xml";
            //std::string strConfigFilePath = "wifi_config.xml";
            //std::wstring strConfigIniFilePath;

            //JGW_GetModuleWorkPathA(strFilePath);
            //JGW_FormatString(strCacheConfigFilePath,"%scache\\%s\\wifi_param.xml",strFilePath.c_str(),JGW_W2A(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName).c_str());
            //JGW_FormatString(strConfigFilePath,"%sconfig\\%s\\wifi_config.xml",strFilePath.c_str(),JGW_W2A(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName).c_str());

            //JGW_FormatWString(strConfigIniFilePath,L"%sconfig\\%s\\equipconfig.ini",JGW_GetApplicationFolder(),mpViewResolverParam->mpPluginInfo->mstrModuleFolderName.c_str());
            ////! strCacheConfigFilePath = strFilePath + "cache\\JGWWiFiTest\\wifi_param.xml";
            ////! strConfigFilePath = strFilePath + "config\\JGWWiFiTest\\wifi_config.xml";

            //if (!wifiTestConfig.LoadWiFiTestConfigXML(m_sWiFiTestConfig,strConfigFilePath)
            //    || !wifiTestConfig.LoadWiFiTestCacheXML(m_sWiFiTestConfig,strCacheConfigFilePath)
            //    || !wifiTestConfig.LoadMonitorConfig(strConfigIniFilePath,m_sWiFiTestConfig))
            //{
            //    MessageBox(m_sWiFiTestConfig.mhMainWnd,_T("配置文件加载失败!!!"),_T("初始化失败"),MB_ICONERROR);
            //    return false;
            //}
        }

        //! WM_HWND_TEST_LIST_MSG
        ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_LIST_MSG,&CCJGW_WiFiCalView::OnTestListMsg);

        if (!mpTestThread) mpTestThread = new CCWiFiCalTestThread;
        if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

        return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class),LPARAM(&msWiFiCalConfig));
    }

    bool CCJGW_WiFiCalView::OnBusyTestStatus()
    {
        mpDoMainList->HomeUp();
        mpDoMainList->SetAllUnSelectedItem();
        mpDoMainList->RemoveAllShow();
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        return true;
    }

    bool CCJGW_WiFiCalView::OnFailTestStatus()
    {
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        //OnSaveTest(false);
        return true;
    }

    bool CCJGW_WiFiCalView::OnPassTestStatus()
    {
        //OnSaveTest(true);
        return true;
    }

    void CCJGW_WiFiCalView::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
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

    void CCJGW_WiFiCalView::OnSaveTest(bool bTestStatus)
    {
        std::wofstream fileStream;
        std::wstring strExcelContent;
        CListTextElementUI* pListTextElementUi = NULL;

        std::wstring strTemp,strFilePath;
        JGW_GetExecSubFolder(strTemp,L"log");
        JGW_FormatWString(strFilePath,L"%s%d.log",strTemp.c_str(),GetTickCount());
        //! GetTickCount
        fileStream.imbue( std::locale("chs") );
        fileStream.open( strFilePath.c_str(),std::ios_base::out | std::ios_base::trunc );

        if (!fileStream.is_open()) return ;

        fileStream << L"测试项\tMin\tValue\tMax\tP/F" << std::endl;

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

        //! CCJGW_MoveTestLogging moveTestLogging;
        //moveTestLogging.MoveTestLogging(L"WIFI_TEST\\",mpTestThread->GetSerialNumber().c_str(),strFilePath.c_str(),bTestStatus);

        return ;
    }
}
