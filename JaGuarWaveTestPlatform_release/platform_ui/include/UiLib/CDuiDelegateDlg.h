#pragma once
#include "UIlib.h"
//#include <ctf_delegate_msg.hpp>
//#include <boost/operators.hpp>

namespace UiLib
{
    class  UILIB_API CDuiDelegateDlg:public CWindowWnd
        , public INotifyUI
        , public IMessageFilterUI
        , public IDialogBuilderCallback
    {
    public:
        CDuiDelegateDlg(void);
        virtual ~CDuiDelegateDlg(void);

        /*		子类必须实现 	*/
        virtual LPCTSTR		GetWindowClassName() const;
        /*    非必需默认实现 */
        virtual bool		InitWindow(){	return true;}
        //! 响应窗口销毁信息
        virtual void        OnCloseWindow() {return ;}

        HWND				CreateDuiDialogWindow(LPCTSTR strSkinFolder
            ,LPCTSTR strSkinFile,DWORD dwStyle = UI_WNDSTYLE_DIALOG,DWORD dwExStyle = WS_EX_STATICEDGE | WS_EX_APPWINDOW,LPCTSTR pstrWindowName = _T("CHDuilibWnd"));
        HWND				CreateDuiDialogWindow(LPCTSTR strSkinFolder
            ,LPCTSTR strSkinFile,LPCTSTR pstrWindowName ,DWORD dwStyle = UI_WNDSTYLE_DIALOG,DWORD dwExStyle = WS_EX_STATICEDGE | WS_EX_APPWINDOW);
        HWND				CreateDuiDialogWindow(HWND hwndParent,LPCTSTR strSkinFolder
            ,LPCTSTR strSkinFile,DWORD dwStyle = UI_WNDSTYLE_DIALOG,DWORD dwExStyle = WS_EX_STATICEDGE | WS_EX_APPWINDOW,LPCTSTR pstrWindowName = _T("CHDuilibWnd")
            ,int x = 0, int y = 0, int cx = 0, int cy = 0);
        HWND				CreateDuiWindow(HWND hwndParent,DWORD dwStyle =0, DWORD dwExStyle =0, LPCTSTR pstrWindowName = _T("CHDuilibWnd"),int x = 0, int y = 0, int cx = 0, int cy = 0);//创建DUI窗口 无客户区
        virtual void		SetSkinFileName(LPCTSTR strSkinFilePath);//设置当前皮肤
        void				SetSkinFolder(LPCTSTR strSkinFolder);//设置当前皮肤路径 不设置默认为当前应用程序路径
        void				SetResourceType(UILIB_RESOURCETYPE eResourceType,DWORD dwResourceID = 0,LPCTSTR strResourceName = _T("ZIPRES"));//设置资源类型

        virtual UINT		GetClassStyle() const;
        virtual LPCTSTR		GetSuperClassName();

    public:
        /*         X3插件G管理接口           */
        /*  mfc SDI Interface*/
        virtual HWND GetWindow() {return GetHWND();}

        //! Create window with a specified control id (eg: AFX_IDW_PANE_FIRST).
        virtual bool CreateWnd(HWND hwndParent, int id) { return true;}

        HWND GetSafeHwnd() const{return GetHWND();}
        //! Destroy window and object.
        virtual void DestroyWindow(){ if (m_hWnd)::DestroyWindow(GetHWND()); m_hWnd = NULL;/*//Close(IDCANCEL);//SendMessage(WM_DESTROY,0,0);*/}

        //! Dispatch a command message.
        virtual bool OnCommand(int id, bool test = false) {return true;}

        //! Dispatch the update notify of a command UI object.
        virtual bool OnUpdateUI(int id, bool& enabled, bool& checked, std::wstring& text) { return true;}

        //! Refresh display.
        virtual void Refresh() {return ;/*this->Refresh();*/}

        virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, void* pHandlerInfo){return TRUE;}
    public:
        virtual void		OnFinalMessage(HWND hWnd);
        virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual CControlUI* CreateControl(LPCTSTR pstrClass) {return NULL;}
        virtual LRESULT		HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
        virtual LRESULT		ResponseDefaultKeyEvent(WPARAM wParam);
        virtual LRESULT		OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
        virtual LRESULT		OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
#if defined(WIN32) && !defined(UNDER_CE)
        virtual LRESULT		OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
        virtual LRESULT		OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
        virtual LRESULT		OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
        virtual LRESULT		OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
        virtual LRESULT		OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif
        virtual LRESULT		OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT		OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
        virtual LRESULT		OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
        virtual LRESULT		OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
        virtual LRESULT		OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
        virtual LRESULT		OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
        virtual LRESULT		OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

    protected:
        LPCTSTR				GetSkinFolder(){return m_strSkinFolder.GetData();}
        LPCTSTR				GetSkinFileName();
        UILIB_RESOURCETYPE	GetResourceType(){return m_eResourceType;}
    protected:
        virtual bool		LoadLayoutFile();
    protected:
        CPaintManagerUI		m_PaintManager;
    protected:
        //DWORD		m_dwStyle;
        DWORD				m_dwResourceID;//资源ID
        CDuiString			m_strSkinFolder;//皮肤文件夹
        CDuiString			m_strSkinFilePath;//皮肤文件路径
        UILIB_RESOURCETYPE	m_eResourceType;//资源类型
        CDuiString			m_strResourceName;//资源名
        bool				m_bIsCreateDuiWnd;//是否创建DUI窗体
    };

}