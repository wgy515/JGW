#pragma once
#include <string>
#include <include/cef_client.h>
#include <include/wrapper/cef_message_router.h>
#include <include/wrapper/cef_helpers.h>
#include <set>
#include <include/wrapper/cef_resource_manager.h>
#include <include/internal/cef_types_wrappers.h>
#include <UiLibCef/UiLibCefDefine.h>
#include "JGW_Cef3HelpHandler.h"
#include <UiLibCef/UiLibCefExportManager.h>

namespace JGW
{
    /*
    JS调用C++ CODE
    JavaScript 代码（运行在渲染进程）=> SendProcessMessage(PID_BROWSER => C++ 代码（运行在浏览器进程）。

    C++  CODE 调用JS
    C++ 代码（运行在浏览器进程）=> SendProcessMessage(PID_RENDERER => JavaScript 代码（运行在渲染进程）


    */
    class UiLibCefHelp : public UiLibCefHelpInterface
    {
    public:
        UiLibCefHelp();
        ~UiLibCefHelp();
    public:
        /**
        * 初始化cef组件 -- 只需要调用一次即可
        * @param[in] cef_temp_path cef临时文件路径
        * @param[in] is_enable_offset_render 是否开启离屏渲染
        * @return bool true 继续运行，false 应该结束程序
        */
        bool Initialize(const std::wstring &cef_temp_path = L"cef_temp", const bool &is_enable_offset_render = false);

        //! CefRendererDelegate 添加C++回调
        /*
        * 注册JS函数，供JS调用，C++函数
        *
        */
        void AddCefRendererDelegate(void* pCefRendererDelegate);
        /**
        * @brief 加载cef
        * @param[in] hwnd 窗口句柄
        * @param[in] windowName 窗口名
        * @param[in] url html路径（网址）
        * @param[in] rect 浏览器所显示的窗口区域
        * @param[in] bUseBrowserMenu 是否启用浏览器右键菜单
        * @return 返回 true 表示加载成功，false 加载失败
        */
        bool LoadCef(const HWND &hwnd,UiLibCefMsgHandler* pUiLibCefMsgHandler, const std::wstring &url, const RECT &rect, const bool &bUseBrowserMenu = false);
        /**
        * 清理cef组件
        * @return void	无返回值
        */
        void UnInitialize();
        //! 关闭浏览器
		void CloseBrowserToId(int id,const bool &bForceClose = false );
        //! 关闭浏览器
		void CloseAllBrowser(const bool &bForceClose = false);
        //! 关闭浏览器
        void SizeMsgHandler(RECT rect);
        //! 关闭浏览器
		void QuitCefQuitMessageLoop();
        //!
        HWND GetBrowserWindowHandleToId(int id);
        //! 
        void ExecuteJavaScriptToId(int id,const std::wstring& strJsCode);
        //!
        void RegisterCppFunc(JsCallCppFuncBK pJsCallCppFuncBK);
        //! 
        void UnRegisterCppFunc(JsCallCppFuncBK pJsCallCppFuncBK);
    private:
        /**
        * 设置cef初始化参数
        * @param[in] cef_temp_path cef临时文件路径
        * param[in] settings
        */
        void GetCefSetting(const std::wstring &cef_temp_path, CefSettings &settings); 
    private:
        bool m_is_enable_offset_render;
        JGW_Cef3HelpHandler* mpCef3HelpHandler;
    };
}
