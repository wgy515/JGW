#pragma once
#include <include/cef_context_menu_handler.h>
namespace JGW
{
    //! CEF3中的CefContextMenuHandler是用于处理浏览器上下文菜单(右键菜单)的核心接口类，主要功能包括：
    //! 允许开发者自定义或拦截浏览器默认的右键菜单行为
    //! 可通过实现OnBeforeContextMenu等方法修改菜单项
    class CustomMenuHandler : public CefContextMenuHandler
    {
    public:
        CustomMenuHandler(void);
        ~CustomMenuHandler(void);
    public:
        // 菜单显示前触发（可修改/清空菜单项）
        void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefContextMenuParams> params,
            CefRefPtr<CefMenuModel> model) override {
                model->Clear(); // 示例：清空默认菜单
        }

        // 菜单命令执行时触发
        bool RunContextMenu(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefContextMenuParams> params,
            CefRefPtr<CefMenuModel> model,
            CefRefPtr<CefRunContextMenuCallback> callback) override { 
            return false; // 返回false则使用默认处理
        }

        //! 调用以执行从上下文菜单中选择的命令。返回true 命令已处理，默认实现为false。有关具有默认实现的命令id，请参阅cef_menu_id_t。所有
        //! 用户定义的命令id应该在MENU_ID_USER_FIRST和
        //!  MENU_ID_USER_LAST。|参数|将具有与传递给的相同的值
        //!  OnBeforeContextMenu()。不要在这个回调之外保留|参数|的引用。
        bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefContextMenuParams> params,
            int command_id,
            EventFlags event_flags) override {
                return false;
        }

        IMPLEMENT_REFCOUNTING(CustomMenuHandler);
    };


}
