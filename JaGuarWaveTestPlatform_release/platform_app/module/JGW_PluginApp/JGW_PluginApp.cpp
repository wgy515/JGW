// JGW_PluginApp.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "JGW_PluginApp.h"
#include "JGWPluginMainDlg.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CJGW_MESConfig.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include "CJGW_FTM_Json.h"

/************************************************************************/
/*
多路测试方案
一、UI采取独立进程，测试也在独立进程方案中
1、优势UI以及测试完全独立互不干扰
2、劣势首次启动时间过长
二、UI采取非独立进程，测试采用独立进程方案
1、优势现有方案开发所需时间较短
2、劣势现有方案只能执行TSE相关测试，其他测试方案需要重新进行开发

倾向于独立方案一

1、实现JaGuarWaveTestPlatform与插件视图相关的部分功能
2、
*/
/************************************************************************/
bool LoadMESConfig(S_MES_INFO& msMesInfo);
//! 在线或者离线模式 消息ID 0 : 离线模式 1 : 在线模式
//! argv[1] = "{"stationid":"003","clsid":"JGW::CCPEMultiDownloadPluginView","viewloader_id":"2","module_folder":"R100_IQ\Download", "name":"CPEMultiDownloadPlugin.dll" ,"station_name":"多路下载" "image_id":"0","suite_name":"download_suite.xml","MainWindow":"124899663","MES_MODEL": 0,"PluginAppID":1}"
//! 主程序窗口句柄以及插件相关测试信息
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    // {\"stationid\":\"003\",\"clsid\":\"JGW::CCPEMultiDownloadPluginView\",\"viewloader_id\":\"2\",\"module_folder\":\"R100_IQ\\\\Download\", \"name\":\"CPEMultiDownloadPlugin.dll\" ,\"station_name\":\"多路下载\",\"image_id\":\"0\",\"suite_name\":\"download_suite.xml\",\"MainWindow\":\"124899663\",\"MES_MODEL\": 0,\"PluginAppID\":1}
#if 0
    std::wstring strDebugArgv = L"{\"stationid\":\"003\",\"clsid\":\"JGW::CCPEMultiDownloadPluginView\",\"viewloader_id\":\"2\",\"module_folder\":\"R100_IQ\\\\Download\", \"name\":\"CPEMultiDownloadPlugin.dll\" ,\"station_name\":\"多路下载\",\"image_id\":\"0\",\"suite_name\":\"download_suite.xml\",\"MainWindow\":\"124899663\",\"MES_MODEL\": 0,\"PluginAppID\":1}";
    {
        boost::property_tree::ptree item;
        item.put("stationid","003");
        item.put("clsid","JGW::CCPEMultiDownloadPluginView");
        item.put("viewloader_id",2);
        item.put("module_folder","R100_IQ\\Download");
        item.put("name","CPEMultiDownloadPlugin.dll");
        item.put("station_name","多路下载");
        item.put("image_id",0);
        item.put("suite_name","download_suite.xml");
        item.put("MainWindow",124899663);
        item.put("MES_MODEL",0);
        item.put("PluginAppID",1);

        std::stringstream is;
        boost::property_tree::write_json(is,item);
        std::string s = is.str();
        OutputDebugStringA(s.c_str());
        strDebugArgv = JGW::JGW_A2W(s);
    }
#endif
    std::wstring strDebugArgv = lpCmdLine;
    //MessageBox(NULL,strDebugArgv.c_str(),NULL,MB_OK);
    int nPluginAppID = 0;
    HWND hMainTestPlatformWindow = NULL;
    E_MES_MODE eMesMode = E_ONLINE_MES;
    // 初始化MES配置信息
    S_MES_INFO sMesInfo;
    sMesInfo.mbIsForceTerminateProcess = true;
    // 关于NET CSTSE配置暂不使用
    sMesInfo.msCSTSEServerConfig.mbOpenCSTSEServerModule = false;
    sMesInfo.msCSTSEServerConfig.mnCSTSEServerSocketPort = 3008;
    //!  <pln stationid="-1" clsid="JGW::CCTSECalConfigDlg" viewloader_id="2" module_folder="R100_IQ\WiFi_2" name="CTSECalConfigPlugin.dll" station_name="线损校准_夹具2" image_id="2" suite_name="wifi_loss_calbration_1.xml" />
    S_PLUGIN_INFO sPluginInfo;
#if 0
    sPluginInfo.mStationID = L"-1";
    sPluginInfo.mstrClsid = L"JGW::CCTSEThroughTestDlg";
    sPluginInfo.mnViewLoadID = 2;
    sPluginInfo.mstrModuleFolderName = L"PER711-03-H";
    sPluginInfo.mstrPluginName = L"CTSEThroughTestPlugin.dll";
    sPluginInfo.mstrStationName = L"BB主板功能测试";
    sPluginInfo.mstrSuiteXmlName = L"product_screen_suite.xml";
    sPluginInfo.mImageGroupID = 2;
#else
    std::string strTemp;
    JGW::CCJGW_FTM_Json cFTMJson;
    cFTMJson.FromJsonToString(JGW::JGW_W2A(strDebugArgv,CP_UTF8));
    if (cFTMJson.GetJsonValueToKey("stationid",strTemp))
    {
        sPluginInfo.mStationID = JGW::JGW_A2W(strTemp);
    }
    if (cFTMJson.GetJsonValueToKey("clsid",strTemp))
    {
        sPluginInfo.mstrClsid = JGW::JGW_A2W(strTemp);
    }
    if (cFTMJson.GetJsonValueToKey("viewloader_id",strTemp))
    {
        sPluginInfo.mnViewLoadID = (size_t)atoi(strTemp.c_str());
    }
    if (cFTMJson.GetJsonValueToKey("module_folder",strTemp))
    {
        sPluginInfo.mstrModuleFolderName = JGW::JGW_A2W(strTemp);
    }
    if (cFTMJson.GetJsonValueToKey("name",strTemp))
    {
        sPluginInfo.mstrPluginName = JGW::JGW_A2W(strTemp);
    }
    if (cFTMJson.GetJsonValueToKey("station_name",strTemp))
    {
        sPluginInfo.mstrStationName = JGW::JGW_A2W(strTemp,CP_UTF8);
    }
    if (cFTMJson.GetJsonValueToKey("image_id",strTemp))
    {
        sPluginInfo.mImageGroupID = (size_t)atoi(strTemp.c_str());
    }
    if (cFTMJson.GetJsonValueToKey("suite_name",strTemp))
    {
        sPluginInfo.mstrSuiteXmlName = JGW::JGW_A2W(strTemp);
    }
    if (cFTMJson.GetJsonValueToKey("MainWindow",strTemp))
    {
         hMainTestPlatformWindow = (HWND)atoi(strTemp.c_str());
    }
    if (cFTMJson.GetJsonValueToKey("MES_MODEL",strTemp))
    {
        eMesMode = atoi(strTemp.c_str()) == 1 ? E_ONLINE_MES:E_NOTONLINE_MES;
    }
    if (cFTMJson.GetJsonValueToKey("PluginAppID",strTemp))
    {
        nPluginAppID = atoi(strTemp.c_str());
    }
#endif

    //! 加载MES配置文件
    if (!LoadMESConfig(sMesInfo))
    {
        goto EXEC_END;
    }
    sMesInfo.msMesConnectInfo.meMESModel = eMesMode;
    HRESULT Hr = ::CoInitialize(NULL);
    if( FAILED(Hr) ) return 0;
    // 打开主窗口
    {
        CPaintManagerUI::SetInstance(hInstance);
        JGW::CJGWPluginMainDlg* dlg = new JGW::CJGWPluginMainDlg(nPluginAppID,hMainTestPlatformWindow,sMesInfo,sPluginInfo);
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("plugin.xml"),_T("JaGuarWave Plugin Application"),NULL,UI_WNDSTYLE_FRAME,UI_WNDSTYLE_EX_FRAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("plugin.xml"),_T("JaGuarWave Plugin Application"),NULL,UI_WNDSTYLE_FRAME,UI_WNDSTYLE_EX_FRAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("plugin.xml"),_T("JaGuarWave Plugin Application"),NULL,UI_WNDSTYLE_FRAME,UI_WNDSTYLE_EX_FRAME);
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JGW_PLUGINAPP);
        dlg->ShowWindow();
        //dlg->ShowModal();
        //delete dlg;
    }
    CPaintManagerUI::MessageLoop();
    CPaintManagerUI::Term();
    ::CoUninitialize();

EXEC_END:
#ifndef _DEBUG
    if (sMesInfo.mbIsForceTerminateProcess)
    {
        HANDLE hp   =   OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,GetCurrentProcessId()); 
        TerminateProcess(hp,0);
    }
#endif

    return 0;
}

bool LoadMESConfig(S_MES_INFO& msMesInfo)
{
    msMesInfo.msMesConnectInfo.meMESModel = E_ONLINE_MES;
    msMesInfo.msMesConnectInfo.meMESStatus = E_MES_UNKNOWN_STATUS;
    msMesInfo.msMesConnectInfo.mstrMESConfigFilePath = JGW::JGW_GetApplicationFolder();
    msMesInfo.msMesConnectInfo.mstrMESConfigFilePath += L"jgw_mes_config.data";
    msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig = JGW::JGW_GetApplicationFolder();
    msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig += L"jgw_mes_workstage.xml";
    Log4WD_F(L"MES Config File Path: %s,MES Work Stage Config File Path: %s",msMesInfo.msMesConnectInfo.mstrMESConfigFilePath.c_str(),msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig.c_str());

    JGW::CCJGW_MESConfig config;
    //! 加载站点ID以及站点名称
    config.LoadMESWorkStageXmlConfig(msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig,&msMesInfo);
    //! 加载MES配置文件
    if (!config.LoadMESIniConfig(msMesInfo.msMesConnectInfo.mstrMESConfigFilePath,&msMesInfo))
    {
        ::MessageBox(NULL,GetString(E_LOAD_MESCONFIG_FAIL),_T("MES Config"),MB_ICONERROR);
        return false;
    }
    return true;
}

