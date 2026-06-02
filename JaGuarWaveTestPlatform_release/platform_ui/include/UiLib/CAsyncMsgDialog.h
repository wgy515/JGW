#pragma once
#include "UIlib.h"
#include <UiLib/JGW_AsyncWndMessage.h>

#define UILIB_STRING(a) #a  
// return _T(UILIB_STRING(DUIL_B_##class));
#define UILIB_GetWindowClassName(class) \
    LPCTSTR GetWindowClassName() const\
{\
    return _T("DUIL_B_")_T(#class);\
}


namespace JGW
{
    class UILIB_API CCAsyncMsgDialog:public CWindowWnd
        ,public INotifyUI //! 窗口事件消息
        /*,public IMessageFilterUI*/ //! 按键消息过滤事件
        ,public IDialogBuilderCallback  //! 自定义控件生成 返回 CreateControl
    {
    public:
        CCAsyncMsgDialog(void);
        ~CCAsyncMsgDialog(void);
  
        DUI_DECLARE_MESSAGE_MAP_B()
    protected:
        //! 创建窗口事件
        virtual void OnInitWindow();
        //! 获取窗口类名
        virtual LPCTSTR GetWindowClassName() const = 0;
        //! 窗口控件事件通知
        virtual void OnNotify(TNotifyUI& msg) = 0;
        //! 响应窗口销毁信息
        virtual void OnCloseWindow();
		//! 处理拦截消息，不往下传送到父窗口
		virtual LRESULT HandleInterceptCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    public:
        void SetResourceType(UILIB_RESOURCETYPE eResourceType,DWORD dwResourceID = 0,LPCTSTR strResourceName = _T("ZIPRES"));//设置资源类型
        //! 创建无边框窗体
        HWND CreateNoBorderStyleWindow(LPCTSTR strSkinFolder
            ,LPCTSTR strSkinFile,LPCTSTR pstrWindowName,HWND hwndParent = NULL
            ,DWORD dwStyle = UI_WNDSTYLE_FRAME,DWORD dwExStyle = WS_EX_STATICEDGE | WS_EX_APPWINDOW
            );
        //! 创建无边框窗体
        HWND CreateNoBorderStyleWindowToRect(LPCTSTR strSkinFolder
            ,LPCTSTR strSkinFile,LPCTSTR pstrWindowName,const RECT rc,HWND hwndParent = NULL
            ,DWORD dwStyle = UI_WNDSTYLE_FRAME,DWORD dwExStyle = WS_EX_STATICEDGE | WS_EX_APPWINDOW);
        //! 窗口无边框对话框
        HWND CreateNoBorderStyleDialog(LPCTSTR strSkinFolder
            ,LPCTSTR strSkinFile,LPCTSTR pstrWindowName,HWND hwndParent,
            DWORD dwStyle = UI_WNDSTYLE_FRAME,DWORD dwExStyle = WS_EX_STATICEDGE | WS_EX_APPWINDOW
            );
        //! 异步消息
        void PutAsyncMessage(UINT msgid,WPARAM wParam = NULL,LPARAM lParam = NULL);
        //! 异步字符串消息
        void PutAsyncStringMessage(UINT msgid,const wchar_t* strMsg,LPARAM lParam = NULL);
        //! 异步字符串消息
        void PutAsyncWparamStringMessage(UINT msgid,WPARAM wParam,const wchar_t* strMsg);

        void SetAsyncFiFoTimeInterval(size_t nmsTime);

    protected:
        CCJGW_AsyncWndMessage* GetAsyncWndMessage() { return &mAsyncWndMessage; }
    private:
        void OnHandleMessageMap(TNotifyUI& msg);
#if 0
        void OnCloseBtn(TNotifyUI& msg);

        void OnMinBtn(TNotifyUI& msg);

        void OnRestoreBtn(TNotifyUI& msg);

        void OnMaxBtn(TNotifyUI& msg);
#endif
    private:
        virtual CControlUI* CreateControl(LPCTSTR pstrClass);
        virtual void        Notify(TNotifyUI& msg);
        //! 消息分发
        virtual LRESULT     HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        //! 窗口销毁的消息
        virtual void        OnFinalMessage(HWND hWnd);
        //! 
        virtual LRESULT     OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
        virtual LRESULT		OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
        virtual LRESULT		OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
        virtual LRESULT		OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual void        ExecuteFIFOEventMessage();
        virtual LRESULT     HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		
    private:
        LPCTSTR				GetSkinFolder(){ return m_strSkinFolder.GetData(); }
        LPCTSTR				GetSkinFileName() { return m_strSkinFileName.GetData(); }
        virtual UILIB_RESOURCETYPE GetResourceType() const { return meUilibResourceType; }
        virtual DWORD       GetResourceID() const { return m_dwResourceID; }
        virtual UINT        GetClassStyle() const { return m_dwClassStyle; }
    protected:
        bool				m_bIsCreateDuiWnd;//是否创建DUI窗体
        UILIB_RESOURCETYPE  meUilibResourceType;
        LPBYTE              m_lpResourceZIPBuffer;
        DWORD				m_dwResourceID;//资源ID
        UINT                m_dwClassStyle;
        size_t              m_nAsyncFiFoTime;
#if 0
#pragma region 窗口关闭按钮
        CButtonUI*		mpCloseBtn;
        CButtonUI*		mpMaxBtn;
        CButtonUI*		mpRestoreBtn;
        CButtonUI*		mpMinBtn;
#pragma endregion 窗口关闭按钮
#endif
        //CDuiString		    ZipSkinPath;
        CDuiString			m_strResourceName;//资源名
        CDuiString			m_strSkinFolder;//皮肤文件夹
        CDuiString			m_strSkinFileName;//皮肤文件路径
        CPaintManagerUI		m_PaintManager;
        CCJGW_AsyncWndMessage mAsyncWndMessage;
    };

}

