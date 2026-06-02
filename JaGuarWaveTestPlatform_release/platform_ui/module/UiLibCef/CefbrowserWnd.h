#pragma once
#include <UiLib/CAsyncMsgDialog.h>

#define CEFBROWSER_LINE_WND   L"CefbrowserLineWnd"

#define WM_SETBROWSER_ID (WM_USER + 111)
#define WM_TIMER_VEHICLE (WM_USER + 112)
#define WM_LOAD_CEF (WM_USER + 109)
namespace JGW
{
    class CCefbrowserWnd : public WindowImplBase
    {
    public:
        CCefbrowserWnd(const RECT &rect);
        ~CCefbrowserWnd(void);
    public:
        // 初始化资源管理器
        virtual void InitWindow() override;
        virtual CDuiString GetSkinFile() override;
        //DUI_DECLARE_MESSAGE_MAP()
        //virtual void OnClick(TNotifyUI& msg);
        virtual LPCTSTR GetWindowClassName() const override;
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
        virtual void OnFinalMessage(HWND hWnd) override;
        virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    private:
        // 初始化
        void Init();
        UINT UseTable(const INT32 &ichoose, const std::wstring& strText, const std::wstring& wstrTitle = L"警告");
        void chgWndPos(RECT rect);
        void OnLoadCEFWnd();
        void AutoRefresh();
    private:
        void OnSizeChange();
    private:
        RECT m_rect;
        INT32 m_iBrowserID;
    };
}
