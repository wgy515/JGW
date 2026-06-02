// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>
#include <map>
#include <string>
#include <include/cef_app.h>
#include <include/wrapper/cef_resource_manager.h>
// TODO: 在此处引用程序需要的其他头文件
#include <include/wrapper/cef_message_router.h>
#include <set>

typedef std::map<std::string, std::string> StringResourceMap;
typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

 // Set up the resource manager for tests.
extern void SetupResourceManager(CefRefPtr<CefResourceManager> resource_manager,StringResourceMap* string_resource_map);
// Create all CefMessageRouterBrowserSide::Handler objects. They will be
// deleted when the ClientHandler is destroyed.
extern void CreateMessageHandlers(MessageHandlerSet& handlers);

//! 窗口句柄资源ID
extern int gnMainIconResId;