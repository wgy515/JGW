// JGW_Cef3PositioningSystem.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "JGW_Cef3PositioningSystem.h"
#include <include/cef_app.h>
#include <JGW_Cef3Plugin/JGW_Cef3PluginExport.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

#include "CJGW_Cef3PositioningSystemUDPScokect.h"
#include "CJGW_PositioningSystemProtocol.h"
#include "CJGW_Cef3BrowserEventHelp.h"

JGW::CCJGW_Cef3BrowserEventHelp gCef3BrowserEventHelp;

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
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
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
	JGW::JGW_FormatWString(strHtmlZipFilePath,L"%s\\%s",JGW::JGW_GetApplicationFolder(),L"Cef3PositioningSystem.zip");


	JGW::JGW_FormatWString(strIniFilePath,L"%s%s",JGW::JGW_GetApplicationFolder(),L"Cef3PositioningSystem.ini");
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
		//! ZIP源 0 ： 默认是文件源 1 ：程序的资源文件  ZIP_SOURCE
		if (1 == _ttoi(GetIniKeyValue(L"MAIN",L"ZIP_SOURCE",strIniFilePath).c_str()))
		{
			SetZipResResource(hInstance,IDR_ZIPRES1,L"ZIPRES",strZipFilePassWord.c_str());
		}
		else
		{
			SetZipFilePath(strHtmlZipFilePath.c_str(),strZipFilePassWord.c_str());
		}

	}
	JGW::CCJGW_Cef3PositioningSystemUDPScokect cPositioningSystemUDPScokect;
	if (!cPositioningSystemUDPScokect.StartCef3UDPServer(JGW::CCJGW_PositioningSystemProtocol::OnRecvPositioningSystemProtocolMsg))
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
	//RegisterCppFuncToJs(L"GetData",GetData);
	JGW::CCJGW_PositioningSystemProtocol cPositioningSystemProtocol(&gCef3BrowserEventHelp,&cPositioningSystemUDPScokect);
	if (!cPositioningSystemProtocol.StartPositioningSystemProtocolThread())
	{
		MessageBox(NULL,cPositioningSystemProtocol.GetErrorMessage().c_str(),L"Error",MB_ICONERROR);
		return -1;
	}

	//! SetData
	//HWND hWindow = FindWindow(NULL,NULL);
	//SetClassLong(FindWindow(NULL,NULL), GCL_HICON, (LONG)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JGW_CEF3MAINPROCESSES))); // 更换主图标:ml-citation{ref="5,10" data="citationList"}

	// 执行消息循环，此时会堵塞，直到CefQuitMessageLoop()函数被调用。
	CefRunMessageLoop();

	RemoveBrowserEventDelegate(&gCef3BrowserEventHelp);
	cPositioningSystemProtocol.StopPositioningSystemProtocolThread();
	cPositioningSystemUDPScokect.StopCef3UDPServer();
	
	// Shut down CEF.
	// 关闭CEF
	CefShutdown();

	return 0;
}
