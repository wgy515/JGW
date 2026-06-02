#include "StdAfx.h"
#include "PluginAppWindowUI.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <sstream>

namespace JGW
{

    CPluginAppWindowUI::CPluginAppWindowUI(void) : mhPluginAppWindow(NULL)
    {
        
    }

    void CPluginAppWindowUI::SendPluginAppMsg(int nMsg,WPARAM wParam,LPARAM lParam)
    {
        if (NULL != mhPluginAppWindow) SendMessage(mhPluginAppWindow,nMsg,wParam,lParam);
    }

    CPluginAppWindowUI::~CPluginAppWindowUI(void)
    {
        StopPluginAppExecute();
    }

    LPCTSTR CPluginAppWindowUI::GetClass() const
    {
        return _T("PluginAppWindowU");
    }

    LPVOID CPluginAppWindowUI::GetInterface(LPCTSTR pstrName)
    {
        if( _tcscmp(pstrName, DUI_CTR_PLUGINAPPWINDOWUI) == 0 ) return static_cast<CPluginAppWindowUI*>(this);
        return CControlUI::GetInterface(pstrName);
    }

    void CPluginAppWindowUI::SetPos(RECT rc)
    {
        CControlUI::SetPos(rc);
        if (NULL != mhPluginAppWindow)
        {
            ::MoveWindow(mhPluginAppWindow,rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,TRUE);
        }
    }

    void CPluginAppWindowUI::DoEvent(TEventUI& event)
    {
        RECT rc = GetPos();
        POINT pt = {event.ptMouse.x - rc.left, event.ptMouse.y - rc.top};
        CControlUI::DoEvent(event);
    }

    void CPluginAppWindowUI::PaintBkImage(HDC hDC)
    {
        CControlUI::PaintStatusImage(hDC);
        RECT rc = GetPos();
        RECT rcPluginAppWindow = {0};
        ::GetWindowRect(mhPluginAppWindow,&rcPluginAppWindow);
        if (NULL != mhPluginAppWindow 
            && (rcPluginAppWindow.bottom != rc.bottom 
            || rcPluginAppWindow.top != rc.top 
            || rcPluginAppWindow.left != rc.left 
            || rcPluginAppWindow.right != rc.right))
        {
            ::MoveWindow(mhPluginAppWindow,rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,TRUE);
        }
    }

    bool CPluginAppWindowUI::StartPluginAppExecute(int nIndex,HWND hMianWindow,int nPluginAppID,int nMesMode,const LPS_MultiPluginAppConfig psMultiPluginAppConfig)
    {
        std::wstring strPluginAppWindowPath(JGW_GetApplicationFolder());
#ifdef _DEBUG
        strPluginAppWindowPath +=  L"JGW_PluginApp_d.exe";
#else
        strPluginAppWindowPath +=  L"JGW_PluginApp.exe";
#endif
        

        std::wstring strArg;
        std::wstring strWorkDir = JGW_GetPathOfFile(strPluginAppWindowPath.c_str());
        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};
#if 0
        boost::property_tree::ptree item;
        item.put("stationid","003");
        item.put("clsid","JGW::CCPEMultiDownloadPluginView");
        item.put("viewloader_id",2);
        item.put("module_folder","R100_IQ\\Download");
        item.put("name","CPEMultiDownloadPlugin.dll");
        item.put("station_name","¶àÂ·ÏÂÔØ");
        item.put("image_id",0);
        item.put("suite_name","download_suite.xml");
        item.put("MainWindow",(int)hMianWindow);
        item.put("MES_MODEL",nMesMode);
        item.put("PluginAppID",nPluginAppID);

        std::stringstream is;
        boost::property_tree::write_json(is,item);
        std::string s = is.str();
        OutputDebugStringA(s.c_str());
#else
        std::wstring strSuiteName;
        std::wstring strSuiteNameFilePath = JGW_GetCacheConfigFilePath(psMultiPluginAppConfig->mstrModuleFolder,psMultiPluginAppConfig->mstrSuiteName);
        JGW_FormatWString(strSuiteName,L"%s.%d",strSuiteNameFilePath.c_str(),nIndex);

        if (!JGW_FileExistsToFilePath(strSuiteName.c_str()))
        {
            CopyFileW(strSuiteNameFilePath.c_str(),strSuiteName.c_str(),FALSE );
        }
        if (!JGW_FileExistsToFilePath(strSuiteName.c_str())) return false;

        JGW_FormatWString(strSuiteName,L"%s.%d",psMultiPluginAppConfig->mstrSuiteName.c_str(),nIndex);
        std::wstring strTemp = JGW_GetFormatWString(L"{\"stationid\":\"%s\",\"clsid\":\"%s\",\"viewloader_id\":\"%d\",\"module_folder\":\"%s\",\"name\":\"%s\",\"station_name\":\"%s\",\"image_id\":\"0\",\"suite_name\":\"%s\",\"MainWindow\":\"%d\",\"MES_MODEL\":\"%d\",\"PluginAppID\":\"%d\"}",psMultiPluginAppConfig->mstrStationID.c_str(),psMultiPluginAppConfig->mstrClsid.c_str(),psMultiPluginAppConfig->mnViewLoaderID,psMultiPluginAppConfig->mstrModuleFolder.c_str(),psMultiPluginAppConfig->mstrPluginName.c_str(),psMultiPluginAppConfig->mstrStationName.c_str(),strSuiteName.c_str(),(int)hMianWindow,nMesMode,nPluginAppID);
        /* "{
        "stationid": "003",
        "clsid": "JGW::CCPEMultiDownloadPluginView",
        "viewloader_id": "2",
        "module_folder": "R100_IQ\\Download",
        "name": "CPEMultiDownloadPlugin.dll",
        "station_name": "\u00B6\u00E0\u00C2\u00B7\u00CF\u00C2\u00D4\u00D8",
        "image_id": "0",
        "suite_name": "download_suite.xml",
        "MainWindow": "658864",
        "MES_MODEL": "0",
        "PluginAppID": "1"
        }
        "*/
#endif
        JGW_FormatWString(strArg,L" %s",strTemp.c_str());
        si.cb = sizeof(STARTUPINFO);
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;;
        si.wShowWindow = SW_SHOW;

        if (!CreateProcess (
            strPluginAppWindowPath.c_str(),
            const_cast<wchar_t*>(strArg.c_str()),
            NULL, NULL,
            TRUE, 0,
            NULL,strWorkDir.c_str(),
            &si, &pi))
        {
            return false;
        }
        return true;
    }

    void CPluginAppWindowUI::SetPluginAppWindow(HWND hPluginAppWindow)
    {
        mhPluginAppWindow = hPluginAppWindow;
    }

    void CPluginAppWindowUI::StopPluginAppExecute()
    {
        if(NULL != mhPluginAppWindow) SendMessage(mhPluginAppWindow,WM_CLOSE,NULL,NULL);
        mhPluginAppWindow = NULL;
    }
}
