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
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 在此处引用程序需要的其他头文件
namespace JGW
{
	enum E_JGW_WLAN_TYPE
	{
		E_WLAN_UNKNOW_TYPE = -1,
		E_SCAN_WLAN = 0,
		E_CONNECT_WLAN = 1,
		E_DISCONNECT_WLAN = 2,
        E_CONNECT_PUMAS = 3, //! 连接PUMAS 
        E_COMMAND_PUMAS = 4, //! 发送PUMAS命令
        E_DISCONNECT_PUMAS = 5, //! pumas
        E_CHECK_WLAN_CONNECT = 6, //! 检查11AD连接上去后，11AD与网关的通讯状态
        //! ShellExecute 运行一个外部程序
        E_SHELL_EXECUTE = 7
	};

    extern HINSTANCE ghInstance;
}

#define JGW_TAG "wlan"
#include <JGW_Log4cplusImplPlugin\log4cplusimpl.h>