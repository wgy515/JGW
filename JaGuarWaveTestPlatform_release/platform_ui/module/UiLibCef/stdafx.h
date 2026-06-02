// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>



// TODO: 在此处引用程序需要的其他头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


//! CEF3是多进程架构的，CEF3进程主要有一个Browser（浏览器）进程和多个Renderer（渲染）进程。Browser被定义为主进程，负责窗口管理，网络请求，网页管理 、网络交互。browser从服务器器请求到了响应，将html文本发送给Renderer 进程，render进程加载html，进行渲染，展示网页的内容；除此之外，Renderer进程还负责Js Binding和对Dom节点的访问。Browser和Renderer进程可以通过发送异步消息进行双向通信。主应用程序很大，加载时间比较长，或者不能在非浏览器进程里使用，则宿主程序可使用独立的可执行文件去运行这些Renderer进程。这可以通过配置CefSettings.browser_subprocess_path变量做到。
//! Browser进程中包含如下主要的线程：
//! TID_UI 线程是浏览器的主线程。如果应用程序在调用调用CefInitialize()时，传递CefSettings.multi_threaded_message_loop=false，这个线程也是应用程序的主线程。
//! TID_IO 线程主要负责处理IPC消息以及网络通信。
//! TID_FILE 线程负责与文件系统交互。