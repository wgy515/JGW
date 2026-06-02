#include <CUiLibWnd.h>

namespace JGW
{
    CCUiLibWnd::CCUiLibWnd(void):meUilibResourceType(UILIB_FILE)
        ,m_strResourceName(_T("ZIPRES"))
        ,m_bIsCreateDuiWnd(false)
        ,m_dwClassStyle(UI_CLASSSTYLE_FRAME|CS_DBLCLKS)
    {
    }


    CCUiLibWnd::~CCUiLibWnd(void)
    {
        m_PaintManager.GetTrayObject().DeleteTrayIcon();
    }

    DUI_BASE_BEGIN_MESSAGE_MAP_B(CCUiLibWnd)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"closebtn",OnCloseBtn)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"minbtn",OnMinBtn)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"maxbtn",OnMaxBtn)
        DUI_ON_MSGTYPE_CTRLNAME_B(DUI_MSGTYPE_CLICK,L"restorebtn",OnRestoreBtn)
     DUI_END_MESSAGE_MAP_B()

    

    void CCUiLibWnd::OnInitWindow()
    {

    }

    void CCUiLibWnd::OnCloseWindow()
    {

    }

    void CCUiLibWnd::OnMinBtn(TNotifyUI& msg)
    {
        SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,NULL);
    }

    void CCUiLibWnd::OnCloseBtn(TNotifyUI& msg)
    {
        Close(IDCANCEL);
        //! PostQuitMessage(0);
    }

    void CCUiLibWnd::OnMaxBtn(TNotifyUI& msg)
    {
        SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,NULL);
    }

    void CCUiLibWnd::OnRestoreBtn(TNotifyUI& msg)
    {
        SendMessage(WM_SYSCOMMAND,SC_RESTORE,NULL);
    }

    void CCUiLibWnd::OnHandleMessageMap(TNotifyUI& msg)
    {
        const DUI_MSGMAP_B* pMessageMap = GetMessageMap();
        const DUI_MSGMAP_ENTRY_B* pEntries = NULL;
        while (pMessageMap)
        {
            pEntries = pMessageMap->lpEntries;
            while (DuiSig_end != pEntries->nSig)
            {
                if (msg.sType == pEntries->sMsgType && (msg.pSender->GetName() == pEntries->sCtrlName || pEntries->sCtrlName.IsEmpty()))
                {
                    if (pEntries->pfn) 
                        (this->*pEntries->pfn)(msg);
                    else
                    {
                        PostMessage(pEntries->nMsgID,pEntries->wParam,pEntries->lParam);
                    }
                }
                pEntries ++;
            }
            pMessageMap = (*pMessageMap->pfnGetBaseMap)();   
        }
    }

    void CCUiLibWnd::Notify(TNotifyUI& msg)
    {
        OnHandleMessageMap(msg);
#if 0
        if(msg.sType == DUI_MSGTYPE_CLICK)
        {
            if(msg.pSender == mpMinBtn)
            {
                SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,NULL);
                return ;
            }
            else if (msg.pSender == mpCloseBtn)
            {
                PostQuitMessage(0);
                return ;
            }
            else if (msg.pSender == mpMaxBtn)
            {
                SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,NULL);
                return ;
            }
            else if (msg.pSender == mpRestoreBtn)
            {
                SendMessage(WM_SYSCOMMAND,SC_RESTORE,NULL);
                return ;
            }
        }
#endif
        OnNotify(msg);
    }

    void CCUiLibWnd::SetResourceType(UILIB_RESOURCETYPE eResourceType,DWORD dwResourceID /* = 0 */,LPCTSTR strResourceName /* = _T("ZIPRES") */)
    {
        meUilibResourceType = eResourceType;
        m_dwResourceID = dwResourceID;
        m_strResourceName = strResourceName;
    }

    HWND CCUiLibWnd::CreateNoBorderStyleWindow(LPCTSTR strSkinFolder ,LPCTSTR strSkinFile,LPCTSTR pstrWindowName,HWND hwndParent /* = NULL  */,DWORD dwStyle /* = UI_WNDSTYLE_FRAME */,DWORD dwExStyle /* = WS_EX_STATICEDGE | WS_EX_APPWINDOW */ )
    {
        m_strSkinFileName = strSkinFile;
        m_strSkinFolder = strSkinFolder;
        m_bIsCreateDuiWnd = true;
        m_dwClassStyle = UI_CLASSSTYLE_FRAME|CS_DBLCLKS;
        return CreateDuiWindow(hwndParent,pstrWindowName,dwStyle,dwExStyle);
    }

    HWND CCUiLibWnd::CreateNoBorderStyleDialog(LPCTSTR strSkinFolder ,LPCTSTR strSkinFile,LPCTSTR pstrWindowName,HWND hwndParent, DWORD dwStyle /* = UI_WNDSTYLE_FRAME */,DWORD dwExStyle /* = WS_EX_STATICEDGE | WS_EX_APPWINDOW */ )
    {
        m_strSkinFileName = strSkinFile;
        m_strSkinFolder = strSkinFolder;
        m_bIsCreateDuiWnd = true;
        m_dwClassStyle = UI_CLASSSTYLE_DIALOG;
        return CreateDuiWindow(hwndParent,pstrWindowName,dwStyle,dwExStyle);
    }

    LRESULT CCUiLibWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(m_PaintManager.GetEventSource()){ 
            TEventUI mEvent = {0};
            mEvent.Type			= uMsg;
            mEvent.lParam		= lParam;
            mEvent.wParam		= wParam;
            mEvent.ptMouse.x	= 0;
            mEvent.ptMouse.y	= 0;
            mEvent.wKeyState	= 0;
            mEvent.dwTimestamp	= GetTickCount();
            mEvent.pSender		= NULL;
            mEvent.chKey		= NULL;

            bHandled = !m_PaintManager.GetEventSource()(&mEvent);
            return bHandled;
        }
        bHandled = FALSE;
        return 0;
    }

    void CCUiLibWnd::OnFinalMessage(HWND hWnd)
    {
        m_PaintManager.ReapObjects(m_PaintManager.GetRoot());
        m_PaintManager.RemoveNotifier(this);
        //OnCloseWindow();
    }

    LRESULT CCUiLibWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;

        switch(uMsg)
        {
            case WM_CLOSE:          OnCloseWindow(); bHandled = FALSE; break;
            case WM_QUIT:           OnFinalMessage(m_hWnd); bHandled = FALSE; break;
            case WM_CREATE:         lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
            case WM_NCHITTEST:	    if(m_bIsCreateDuiWnd) lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); else bHandled = FALSE;break;
            case WM_NCACTIVATE:		if(m_bIsCreateDuiWnd) lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); else bHandled = FALSE; break;
            case WM_NCCALCSIZE:		if(m_bIsCreateDuiWnd) lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); else bHandled = FALSE; break;
            case WM_NCPAINT:		if(m_bIsCreateDuiWnd) lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); else bHandled = FALSE; break;
            case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
            case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
            case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
            case  WM_TIMER:         lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
            default:                bHandled = FALSE; break;
        }

        if (bHandled) return lRes;

        lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
        if (bHandled)
            return lRes;

        if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
            return lRes;

        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }

    LRESULT CCUiLibWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if ( m_bIsCreateDuiWnd )
        {
            LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
            styleValue &= ~WS_CAPTION;
            ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        }

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);
        ::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
            rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

        m_PaintManager.Init(m_hWnd);
        CDialogBuilder builder;

        if (m_PaintManager.GetResourcePath().IsEmpty())
        {	// 允许更灵活的资源路径定义
            CDuiString strResourcePath	=	m_PaintManager.GetInstancePath();
            strResourcePath	+=	m_strSkinFolder.GetData();
            m_PaintManager.SetResourcePath(strResourcePath.GetData());
        }

        switch(GetResourceType())
        {
        case UILIB_ZIP:
            m_PaintManager.SetResourceZip(m_strResourceName.GetData(), true);
            break;
        case UILIB_ZIPRESOURCE:
            {
                HRSRC hResource = ::FindResource(m_PaintManager.GetResourceDll(), MAKEINTRESOURCE(m_dwResourceID), m_strResourceName.GetData());
                if( hResource == NULL )
                    return false;
                DWORD dwSize = 0;
                HGLOBAL hGlobal = ::LoadResource(m_PaintManager.GetResourceDll(), hResource);
                if( hGlobal == NULL ) 
                {
#if defined(WIN32) && !defined(UNDER_CE)
                    ::FreeResource(hResource);
#endif
                    return false;
                }
                dwSize = ::SizeofResource(m_PaintManager.GetResourceDll(), hResource);
                if( dwSize == 0 )
                    return false;

                m_PaintManager.SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);

#if defined(WIN32) && !defined(UNDER_CE)
                ::FreeResource(hResource);
#endif
            }
            break;
        }

        CControlUI* pRoot=NULL;
        if (GetResourceType()==UILIB_RESOURCE)
        {
            STRINGorID xml(m_dwResourceID);
            pRoot = builder.Create(xml, _T("xml"), NULL/*this*/, &m_PaintManager);
        }
        else
            pRoot = builder.Create(m_strSkinFileName.GetData(), (UINT)0, NULL/*this*/, &m_PaintManager);
        ASSERT(pRoot);
        if (pRoot == NULL)
        {
            MessageBox(NULL,_T("加载资源文件失败"),_T("Duilib"),MB_OK|MB_ICONERROR);
            ExitProcess(1);
        }
        m_PaintManager.AttachDialog(pRoot);
        m_PaintManager.AddNotifier(this);
#if 0
#pragma region 窗口关闭按钮
        mpCloseBtn					= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
        mpMaxBtn					= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("maxbtn")));
        mpRestoreBtn				= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("restorebtn")));
        mpMinBtn					= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minbtn")));
#pragma endregion 窗口关闭按钮
#endif
        OnInitWindow();

        return 0;
    }

    LRESULT CCUiLibWnd::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
    {
        if( ::IsIconic(*this) ) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
    }

    LRESULT CCUiLibWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LPRECT pRect=NULL;

        if ( wParam == TRUE)
        {
            LPNCCALCSIZE_PARAMS pParam = (LPNCCALCSIZE_PARAMS)lParam;
            pRect = &pParam->rgrc[0];
        }
        else
        {
            pRect = (LPRECT)lParam;
        }

        if ( ::IsZoomed(m_hWnd))
        {	// 最大化时，计算当前显示器最适合宽高度
            MONITORINFO oMonitor = {};
            oMonitor.cbSize = sizeof(oMonitor);
            ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
            CDuiRect rcWork = oMonitor.rcWork;
            CDuiRect rcMonitor = oMonitor.rcMonitor;
            rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

            pRect->top = pRect->left = 0;
            pRect->right = pRect->left + rcWork.GetWidth();
            pRect->bottom = pRect->top + rcWork.GetHeight();
            return WVR_REDRAW;
        }

        return 0;
    }

    LRESULT CCUiLibWnd::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        return 0;
    }

    LRESULT CCUiLibWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);

        if( !::IsZoomed(*this) )
        {
            RECT rcSizeBox = m_PaintManager.GetSizeBox();
            if( pt.y < rcClient.top + rcSizeBox.top )
            {
                if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
                return HTTOP;
            }
            else if( pt.y > rcClient.bottom - rcSizeBox.bottom )
            {
                if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
                return HTBOTTOM;
            }

            if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
        }

        RECT rcCaption = m_PaintManager.GetCaptionRect();
        if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
                if( pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
                    _tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
                    _tcsicmp(pControl->GetClass(), _T("TextUI")) != 0 &&
                    _tcsicmp(pControl->GetClass(), _T("SliderUI")) != 0 )
                    return HTCAPTION;
        }

        return HTCLIENT;
    }

    LRESULT CCUiLibWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;

        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
        CDuiRect rcWork = oMonitor.rcWork;
        CDuiRect rcMonitor = oMonitor.rcMonitor;
        rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

        // 计算最大化时，正确的原点坐标
        lpMMI->ptMaxPosition.x	= rcWork.left;
        lpMMI->ptMaxPosition.y	= rcWork.top;

        lpMMI->ptMaxTrackSize.x =rcWork.GetWidth();
        lpMMI->ptMaxTrackSize.y =rcWork.GetHeight();

        lpMMI->ptMinTrackSize.x =m_PaintManager.GetMinInfo().cx;
        lpMMI->ptMinTrackSize.y =m_PaintManager.GetMinInfo().cy;

        bHandled = FALSE;
        return 0;
    }


    LRESULT CCUiLibWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
        if( !::IsIconic(*this) /*&& (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)*/ ) {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }
#endif
        bHandled = FALSE;
        return 0;
    }

    LRESULT CCUiLibWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (wParam == SC_CLOSE)
        {
            bHandled = TRUE;
            SendMessage(WM_CLOSE);
            return 0;
        }
#if defined(WIN32) && !defined(UNDER_CE)
        BOOL bZoomed = ::IsZoomed(*this);
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
        if( ::IsZoomed(*this) != bZoomed )
        {
#if 0
            // 切换最大化按钮和还原按钮的状态
            if (mpMaxBtn && mpRestoreBtn)
            {
                mpMaxBtn->SetVisible(TRUE == bZoomed);       // 此处用表达式是为了避免编译器BOOL转换的警告
                mpRestoreBtn->SetVisible(FALSE == bZoomed);
            }
#else
            CControlUI* pbtnMax     = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));       // 最大化按钮
            CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

            // 切换最大化按钮和还原按钮的状态
            if (pbtnMax && pbtnRestore)
            {
                pbtnMax->SetVisible(TRUE == bZoomed);       // 此处用表达式是为了避免编译器BOOL转换的警告
                pbtnRestore->SetVisible(FALSE == bZoomed);
            }
#endif
        }
#else
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
        if (SC_RESTORE == (wParam & 0xfff0))
        {
            bHandled = FALSE;
        }
        return lRes;
    }

    LRESULT CCUiLibWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        return 0;
    }
}

