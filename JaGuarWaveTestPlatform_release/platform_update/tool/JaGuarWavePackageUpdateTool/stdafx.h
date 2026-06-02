// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>
#include <string>
#include <UiLib/CAsyncMsgDialog.h>
#include <JaGuarWaveHelp/CJGW_HelpUpdateDefine.h>
// TODO: 在此处引用程序需要的其他头文件
extern HINSTANCE ghInstance;
#define WM_THREAD_PARSE_PROGRAMFILES_MSG (WM_USER + 1036)
//!  Operation complete
#define WM_HWND_OPERATION_COMPLETE_MSG (WM_USER + 1037)
//!
#define WM_THREAD_UPLOAD_PACKAGEUPDATEFILE_MSG (WM_USER + 1038)
namespace JGW
{
    typedef struct
    {
        size_t mnFTPPort;
        std::string mstrFTPHost;
        std::string mstrUserName;
        std::string mstrUserPassword;
        std::wstring mstrFTPRootPath;
        std::wstring mstrFTPLogRootPath;
    }S_UPDATE_FTP_SERVER_INFO,*PS_UPDATE_FTP_SERVER_INFO;

    typedef struct
    {
        bool mbIsNewInstallVersion;

        bool mbIsParseProgramFiles;
        //! 
        HWND mhMainWnd;
        //! 
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        //! 安装程序目录
        std::wstring mstrProgramFiles;
        //! 更新包临时目录
        std::wstring mstrTempUpdatePackageFilePath;
        //!
        std::wstring mstrTempUpdateXmlFilePath;
        //! 订单号
        std::wstring mstrTaskNumber;
        //! 安装版本号
        std::wstring mstrInstallVersion;
        //! 安装目录的更新版本号
        std::wstring mstrCurrentUpdateVersion;
        //! 当前需要更新到版本号，默认比当前更新版本号 + 1，安装版本：JaGuarWaveTestPlatform_V20201112，更新版本JaGuarWaveTestPlatform_V20201112001（JaGuarWaveTestPlatform_V20201112 + 001）
        std::wstring mstrUpdateVersion; 
        //!
        //std::wstring mstrInstallFTPTempXmlPath;
        ////! 
        //std::wstring mstrUpdateFTPTempXmlPath;
        //! 更新服务器相关信息
        S_UPDATE_FTP_SERVER_INFO msUpdateFtpServerInfo;
        ////! 
        S_JGW_HELP_INSTALL_XML_CONFIG msInstallXmlConfig;
        //!
        S_JGW_HELP_UPDATE_XML_CONFIG msUpdateXmlConfig;
        //!
        std::vector<std::wstring> mvUpdateFilePath;
    }S_PACKAGEUPDATE_INFO,*PS_PACKAGEUPDATE_INFO;

    typedef struct
    {
        //! 
        std::wstring mstrProgramFiles;
        //! 
        std::map<std::wstring,std::wstring> mmapFileMD5;
        //! 
        std::vector<std::wstring> mvIgnoreFiles;
    }S_MD5FILE_PARAM,*PS_MD5FILE_PARAM;
}