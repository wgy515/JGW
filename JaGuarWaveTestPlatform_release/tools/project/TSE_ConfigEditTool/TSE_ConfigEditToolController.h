#pragma once
#include <UiLib/CAsyncMsgDialog.h>
//! UI消息处理
namespace JGW
{
    class CTSE_ConfigEditToolDlg;

    class CTSE_ConfigEditToolController
    {
    public:
        CTSE_ConfigEditToolController(CTSE_ConfigEditToolDlg* pView);
        ~CTSE_ConfigEditToolController(void);
    public:
        //! 响应WINDOWS 消息
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        //! 下发按键消息
        virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
        //! 响应视图Log消息
        void OnResponceViewLogInfoMsg(WPARAM wParam, LPARAM lParam);
        //! 
        void OnViewNotify(TNotifyUI& msg);
    private:
        //! 响应EDIT 菜单栏
        void OnEditMenuBtnClick(TNotifyUI& msg);
        //! Project 菜单栏
        void OnProjectMenuBtnClick(TNotifyUI& msg);
        //! Test 菜单栏
        void OnTestMenuBtnClick(TNotifyUI& msg);
        //! Log 菜单栏
        void OnLogMenuBtnClick(TNotifyUI& msg);
        //! add suite btn
        void OnAddSuiteBtnClick(TNotifyUI& msg);
        //! del suite btn
        void OnDelSuiteBtnClick(TNotifyUI& msg);
        //! 响应菜单栏单机
        void OnMenuElementClick(TEventUI* pTEventUI);
        //! 最大化LOG窗口
        void OnMaximizeLogLayoutBtnClick(TNotifyUI& msg);  
    private: //! 响应快捷方式
        //! 新建
        void OnNewMenuElementClick();
        //! 打开
        void OnOpenMenuElementClick();
        //! 保存
        void OnSaveMenuElementClick();
        //! 另存为
        void OnSaveAsMenuElementClick();
        //! 复制
        void OnCopyShortcutClick();
        //!
        void OnPasteShortcutClick();
    private:
        void OnUpdateProjectMenuUIInfo(CMenuWnd* pMenu);
    private:
        bool mbClickProjectMenu;
        CTSE_ConfigEditToolDlg* mpView;
    };
}