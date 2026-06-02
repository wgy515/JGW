// JGW_Cef3MainProcesses.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "JGW_Cef3MainProcesses.h"
#include <include/cef_app.h>
#include <JGW_Cef3Plugin/JGW_Cef3PluginExport.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "CJGW_Cef3UDPServer.h"
//! 重置显示
#define RESET_COMMAND "31C556A5-DD94-4214-8954-E9DCC67B3117"
//! 设置图表最大的显示点数
#define SETTING_MAX_LIST_COUNT "33F55E00-35A5-4B62-9D2B-3BC988AF44BB"
//! StartAutoScroll <Guid("B191FCFF-D841-4093-9FFC-1AB19641FFAC")>
#define START_AUTO_SCROLL "B191FCFF-D841-4093-9FFC-1AB19641FFAC"
//! StopAutoScroll <Guid("CCE46875-6001-459A-AE4F-0B5B85338BEF")>
#define STOP_AUTO_SCROLL "CCE46875-6001-459A-AE4F-0B5B85338BEF"


//#define URL_DEF 

//void AddCefDllToPath(const std::wstring &cef_path)
//{
//    TCHAR path_envirom[4096] = { 0 };
//    GetEnvironmentVariableW(L"path", path_envirom, 4096);
//
//    std::wstring new_envirom(cef_path);
//    new_envirom.append(L";").append(path_envirom);
//    SetEnvironmentVariableW(L"path", new_envirom.c_str());
//
//    std::wstring cmd_path = cef_path + L"\\dummy_cmd.exe";
//    SetEnvironmentVariableW(L"ComSpec", cmd_path.c_str());
//}

namespace JGW
{
    class CCJGW_Cef3BrowserEventHelp : public CCJGW_Cef3BrowserEventDelegate
    {
    public:
        CCJGW_Cef3BrowserEventHelp() : mnBrowserID(-1)
        {

        }
        ~CCJGW_Cef3BrowserEventHelp()
        {

        }
    protected:
        void OnBrowserCreate(int nBrowserID,const std::wstring& strUrl)
        {
            mnBrowserID = nBrowserID;
        }
    public:
        int mnBrowserID;
    };
}

JGW::CCJGW_Cef3BrowserEventHelp gCef3BrowserEventHelp;

const wchar_t* GetData(const wchar_t* szMessage)
{
    return L"{\"station\" : 0,\"time_stamp\" : 13556666,\"iperf_mbps\" : \"2000\",\"connect_status\":2,\"sta_mac\":\"00:12:13:14:15\",\"ap_mac\":\"00:12:13:15:16:18\",\"ssid\":\"PTP_60G_2\",\"rssi\":-50,\"snr\":8,\"tx_mcs\":1,\"rx_mcs\":1,\"rx_sector\":22,\"tx_sector\":22,\"tx_tput_mbps\":15,\"rx_tput_mbps\":15}";
}

void OnRecvUDPBuffer(const std::string& strRecvBuffer)
{
    if (0 == strRecvBuffer.find(RESET_COMMAND))
    {
        ExecuteJaveScript(gCef3BrowserEventHelp.mnBrowserID,L"ResetData()");
    }
    else if (0 == strRecvBuffer.find(SETTING_MAX_LIST_COUNT))
    {
        std::string strTemp (strRecvBuffer);
        JGW::JGW_ReplaceStringA(strTemp,SETTING_MAX_LIST_COUNT,"");
        JGW::JGW_ReplaceStringA(strTemp,":","");
        std::wstring wstrTemp;
        JGW::JGW_FormatWString(wstrTemp,L"SettingMaxListCount(%d);",atoi(strTemp.c_str()) < 10 ? 10 : atoi(strTemp.c_str()));
        ExecuteJaveScript(gCef3BrowserEventHelp.mnBrowserID,wstrTemp.c_str());
    }
    else if (0 == strRecvBuffer.find(START_AUTO_SCROLL))
    {
        ExecuteJaveScript(gCef3BrowserEventHelp.mnBrowserID,L"StartAutoScroll()");
    }
    else if (0 == strRecvBuffer.find(STOP_AUTO_SCROLL))
    {
        ExecuteJaveScript(gCef3BrowserEventHelp.mnBrowserID,L"StopAutoScroll()");
    }
    else
    {
        std::wstring strTemp;
        JGW::JGW_FormatWString(strTemp,L"SetData('%s');",JGW::JGW_A2W(strRecvBuffer).c_str());
        ExecuteJaveScript(gCef3BrowserEventHelp.mnBrowserID,strTemp.c_str());
    }
    //int nBrowserID = GetBrowserIdToUrl(L"file:///G:/git_code_debug/FTM-tools/JaGuarWaveTestPlatform/bin/debug/exec_debug/cef3/resource/dialogs.html");
    //if (nBrowserID > 0)
    //{
    //    std::wstring strTemp;
    //    JGW::JGW_FormatWString(strTemp,L"SetData('%s');",JGW::JGW_A2W(strRecvBuffer).c_str());
    //    ExecuteJaveScript(nBrowserID,strTemp.c_str());
    //}
}

std::wstring GetIniKeyValue(const wchar_t *lpAppName,const wchar_t *lpKeyName,const std::wstring& strIniPath,const wchar_t *lpDefaleName = L"")
{
    wchar_t strKeyValue[512] = {0};
    GetPrivateProfileStringW(lpAppName,lpKeyName,lpDefaleName,strKeyValue,512,strIniPath.c_str());
    return strKeyValue;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    //! 设置渲染进程路径
    std::wstring strSubProcessesPath;
    std::wstring strCefFolder;
    //JGW::JGW_FormatWString(strCefFolder,L"%s%s",JGW::JGW_GetApplicationFolder(),L"cef3");
    //AddCefDllToPath(strCefFolder);
    JGW::JGW_FormatWString(strSubProcessesPath,L"%s\\%s",JGW::JGW_GetApplicationFolder(),L"JGW_Cef3SubProcesses.exe");

    // Run the CEF message loop. This will block until CefQuitMessageLoop() is called.
    InitializeCef3Processes(GetModuleHandle(NULL),false,strSubProcessesPath.c_str(),JGW::JGW_GetApplicationFolder());

    //! load ini config

    std::wstring strIniFilePath;
    std::wstring strZipFilePassWord;

    std::wstring strHtmlUrl;
    JGW::JGW_FormatWString(strHtmlUrl,L"file:///%sresource/%s",JGW::JGW_GetApplicationFolder(),L"prs_info.html");
    JGW::JGW_ReplaceStringW(strHtmlUrl,L"\\",L"/");

    //! 设置HTML资源文件路径
    std::wstring strHtmlZipFilePath;
    JGW::JGW_FormatWString(strHtmlZipFilePath,L"%s\\%s",JGW::JGW_GetApplicationFolder(),L"Cef3MainProcesses.zip");


    JGW::JGW_FormatWString(strIniFilePath,L"%s%s",JGW::JGW_GetApplicationFolder(),L"Cef3MainProcesses.ini");
    if (JGW::JGW_FileExistsToFilePath(strIniFilePath.c_str()))
    {
        std::wstring strTempUrl;
        strTempUrl = GetIniKeyValue(L"MAIN",L"URL",strIniFilePath);
        if (!strTempUrl.empty())
        {
            strHtmlUrl = strTempUrl;
        }
        else
        {
            strTempUrl = GetIniKeyValue(L"MAIN",L"File",strIniFilePath);
            if (!strTempUrl.empty())
            {
                
                JGW::JGW_FormatWString(strHtmlUrl,L"file:///%s",JGW::JGW_RealativePathToAbsPath(strTempUrl.c_str()).c_str());
                JGW::JGW_ReplaceStringW(strHtmlUrl,L"\\",L"/");
            }
        }
        //! ZIP文件密码
        strZipFilePassWord = GetIniKeyValue(L"MAIN",L"ZIP_PASSWORD",strIniFilePath,strZipFilePassWord.c_str());
        if (0 == strZipFilePassWord.compare(L"WLaecXkb02XRQ06EBnn0Bw=="))
        {
            strZipFilePassWord = L"jie@2017";
        }
        //! ZIP文件路径
        strTempUrl = GetIniKeyValue(L"MAIN",L"ZIP_PATH",strIniFilePath);
        if (!strTempUrl.empty())
        {
            strHtmlZipFilePath = JGW::JGW_RealativePathToAbsPath(strTempUrl.c_str());
        }
    }
    

    
    //SetZipResResource(hInstance,IDR_ZIPRES1,L"ZIPRES");
    SetZipFilePath(strHtmlZipFilePath.c_str(),strZipFilePassWord.c_str());

    JGW::CCJGW_Cef3UDPServer cCef3UDPServer;
    if (!cCef3UDPServer.StartCef3UDPServer(OnRecvUDPBuffer))
    {
        return -1;
    }


    //! 创建浏览器窗口
    RECT rect ={0};
 #if 0
    //CreateBrowserHelp(NULL,rect,L"file:///G:/CEF/cef_binary_83.5.0+gbf03589+chromium-83.0.4103.106_windows32/tests/cefclient/resources/dialogs.html");
    //CreateBrowserHelp(NULL,rect,L"client://ziprs.com/dialogs.html");
    //CreateBrowserHelp(NULL,rect,L"https://zipres.com/prs_connect_status.html");
    //CreateBrowserHelp(NULL,rect,L"file:///G:/git_code_debug/FTM-tools/JaGuarWaveTestPlatform/bin/debug/exec_debug/cef3/resource/dialogs.html");
//#ifdef _DEBUG

#ifdef _DEBUG
    std::wstring strFilePath;
    JGW::JGW_FormatWString(strFilePath,L"file:///%sresource/%s",JGW::JGW_GetApplicationFolder(),L"prs_echarts_info.html");
    JGW::JGW_ReplaceStringW(strFilePath,L"\\",L"/");
    CreateBrowserHelp(NULL,rect,strFilePath.c_str());
#else
    CreateBrowserHelp(NULL,rect,L"https://zipres.com/prs_echarts_info.html");
#endif
#endif
    CreateBrowserHelp(NULL,rect,strHtmlUrl.c_str());
    //CreateBrowserHelp(NULL,rect,L"http://127.0.0.1:8020/html/twts_173_carbon/dist/chartjs.bak.html");
    AddBrowserEventDelegate(&gCef3BrowserEventHelp);
    //! 
    RegisterCppFuncToJs(L"GetData",GetData);
    //! SetData
    //HWND hWindow = FindWindow(NULL,NULL);
    //SetClassLong(FindWindow(NULL,NULL), GCL_HICON, (LONG)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JGW_CEF3MAINPROCESSES))); // 更换主图标:ml-citation{ref="5,10" data="citationList"}

    // 执行消息循环，此时会堵塞，直到CefQuitMessageLoop()函数被调用。
    CefRunMessageLoop();

    RemoveBrowserEventDelegate(&gCef3BrowserEventHelp);
    // Shut down CEF.
    // 关闭CEF
    CefShutdown();

    return 0;
}

